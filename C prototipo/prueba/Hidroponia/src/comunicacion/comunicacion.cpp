#include "comunicacion/comunicacion.h"
#include <WiFi.h>                    // WiFi.status(), WiFi.macAddress()
#include <PubSubClient.h>
#include "sensores/sensores.h"       // aporta temperaturaAgua, nivelAgua, etc. :contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}
#include "control/control.h"         // aporta bomba_activa, luces_activas, alarma_activa, modo_automatico :contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3}
#include "rtc/rtc.h"                 // aporta obtenerHora()
#include <ArduinoJson.h>

//================================================
// FLAGS Y CLIENTES
// ===============================================
bool wifiActivo  = true;
bool loRaActivo  = false;
bool bleActivo   = false;
bool forceGsm    = false;

WiFiClient   espClient;
PubSubClient wifiMqttClient(espClient);

TinyGsmClient& gclient = gsmGetClient();
PubSubClient gsmMqttClient(gclient);

//================================================
// INICIALIZACIÓN DE COMUNICACIONES
// ===============================================
void inicializarComunicaciones() {
  // 1) Wi-Fi: carga credenciales y nada más
  inicializarWiFi();

  // 2) GSM: arranca el módulo con APN/USER/PASS
  inicializarGsm(APN, GPRS_USER, GPRS_PASS);

  // 3) LoRa
  if (loRaActivo) conectarLoRa();

  // 4) BLE Mesh
  if (bleActivo) conectarBLEMesh();
}

//================================================
// CONECTAR A REDES Y MQTT
// =============================================== 
void conectarWiFi() {
  Serial.println("Conectando a WiFi...");
  if (!conectarWiFi(redSeleccionada, claveWiFi)) {
    Serial.println("Fallo al conectar con credenciales guardadas");
  }
}

// ----- MQTT por Wi-Fi -----  
void conectarMQTT_WiFi() {
  wifiMqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  Serial.println("Conectando MQTT WiFi...");
  while (!wifiMqttClient.connected()) {
    if (wifiMqttClient.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT WiFi conectado");
      wifiMqttClient.subscribe("hidroponia/control");
    } else {
      Serial.print("Error MQTT WiFi: ");
      Serial.println(wifiMqttClient.state());
      delay(2000);
    }
  }
}

// ----- MQTT por GSM -----  
void conectarGSM_MQTT() {
  gsmMqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  Serial.println("Conectando MQTT GSM...");

  // Generamos un clientId temporal en RAM y lo convertimos a const char*
  String clientIdGsm = String(CLIENT_ID) + "-GSM";
  const char* clientIdCStr = clientIdGsm.c_str();

  while (!gsmMqttClient.connected()) {
    if (estadoGsm() && gsmMqttClient.connect(clientIdCStr, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT GSM conectado");
      gsmMqttClient.subscribe("hidroponia/control");
    } else {
      Serial.print("Error MQTT GSM: ");
      Serial.println(gsmMqttClient.state());
      delay(2000);
    }
  }
}

// ----- Función unificada MQTT -----  
void conectarMQTT() {
  if (forceGsm) {
    conectarGSM_MQTT();
  } else {
    if (wifiActivo && WiFi.status() == WL_CONNECTED) {
      conectarMQTT_WiFi();
    } else if (estadoGsm()) {
      conectarGSM_MQTT();
    }
  }
}

//================================================
// CICLO DE COMUNICACIÓN
// ===============================================
void manejarComunicacion() {
  // Mantiene viva la conexión GSM
  gsmLoop();

  // Escucha/controla MQTT
  conectarMQTT();
  if (!forceGsm && wifiActivo && wifiMqttClient.connected()) {
    wifiMqttClient.loop();
  } else if (gsmMqttClient.connected()) {
    gsmMqttClient.loop();
  }
}

// ----- Envío de datos por MQTT -----  
void enviarDatosMQTT() {
  // 1) Verificar conexión
  if (forceGsm || !(wifiActivo && WiFi.status() == WL_CONNECTED)) {
    if (!gsmMqttClient.connected()) return;
  } else if (!wifiMqttClient.connected()) {
    return;
  }

  // 2) Construir JSON
  DynamicJsonDocument doc(512);
  doc["deviceId"]  = String(WiFi.macAddress());
  doc["timestamp"] = obtenerHora();

  JsonObject sensors  = doc.createNestedObject("sensors");
  sensors["temperaturaAgua"] = temperaturaAgua;
  sensors["temperaturaAire"] = temperaturaAire;
  sensors["humedad"]         = humedad;
  sensors["nivelAgua"]       = nivelAgua;
  sensors["flujoAgua"]       = flujoAgua;
  sensors["luz"]             = luz;
  sensors["gas"]             = gas;
  sensors["corriente"]       = corriente;
  sensors["voltaje"]         = voltaje;
  sensors["potencia"]        = potencia;
  sensors["phValor"]         = phValor;

  JsonObject controls = doc.createNestedObject("controls");
  controls["bomba"]          = bomba_activa;
  controls["luces"]          = luces_activas;
  controls["alarma"]         = alarma_activa;
  controls["modoAutomatico"] = modo_automatico;

  // 3) Serializar y publicar
  char buf[512];
  size_t len = serializeJson(doc, buf);
  if (forceGsm || !(wifiActivo && WiFi.status() == WL_CONNECTED)) {
    gsmMqttClient.publish("hidroponia/datos", buf, len);
  } else {
    wifiMqttClient.publish("hidroponia/datos", buf, len);
  }
}


// ----- LoRa y BLE sin cambios -----
void conectarLoRa() {
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("Error LoRa");
  } else Serial.println("LoRa OK");
}

void conectarBLEMesh() {
  BLEDevice::init(CLIENT_ID);
  Serial.println("BLE Mesh iniciado");
}

void enviarDatosLoRa() {
  if (loRaActivo) {
    LoRa.beginPacket();
    LoRa.print("{\"tempAgua\":"); LoRa.print(temperaturaAgua);
    LoRa.print(",\"nivelAgua\":"); LoRa.print(nivelAgua);
    LoRa.print("}");
    LoRa.endPacket();
  }
}

void recibirDatosLoRa() {
  int size = LoRa.parsePacket();
  if (size) {
    String msg;
    while (LoRa.available()) msg += (char)LoRa.read();
    Serial.println("LoRa recv: " + msg);
  }
}

void enviarDatosBLE() {
  if (bleActivo) {
    // Lógica BLE Mesh aquí
  }
}

void recibirDatosBLE() {
  // Lógica recepción BLE aquí
}
