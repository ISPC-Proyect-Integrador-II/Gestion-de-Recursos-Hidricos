#include "comunicacion.h"
#include "sensores.h"


WiFiClient espClient;
PubSubClient client(espClient);

// Variables de activación/desactivación de comunicación
bool wifiActivo = true;  // WiFi activado por defecto
bool loRaActivo = false; // LoRa desactivado por defecto
bool bleActivo = false;   // BLE Mesh activado por defecto


// Conexión a WiFi
void conectarWiFi() {
    Serial.println("Conectando a WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int intentos = 0;
    while (WiFi.status() != WL_CONNECTED && intentos < 20) {
        delay(500);
        Serial.print(".");
        intentos++;
    }
    Serial.println(WiFi.status() == WL_CONNECTED ? "\nWiFi conectado" : "\nError WiFi");
}

// Conexión a MQTT
void conectarMQTT() {
    client.setServer(MQTT_BROKER, MQTT_PORT);
    Serial.println("Conectando a MQTT...");
    while (!client.connected()) {
        if (client.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("Conectado a MQTT!");
            client.subscribe("hidroponia/control");
        } else {
            Serial.print("Error MQTT: ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

// Conexión a LoRa
void conectarLoRa() {
    LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println("Error al iniciar LoRa");
        return;
    }
    Serial.println("LoRa iniciado correctamente.");
}

// Conexión a BLE Mesh
void conectarBLEMesh() {
    BLEDevice::init("ESP32-Hidroponia");
}

// Manejo de mensajes MQTT
void manejarMQTT() {
    if (!client.connected()) conectarMQTT();
    client.loop();
}

// Enviar datos por MQTT
void enviarDatosMQTT() {
    String datos = "{\"tempAgua\":" + String(temperaturaAgua) + ",\"nivelAgua\":" + String(nivelAgua) + "}";
    client.publish("hidroponia/datos", datos.c_str());
}

// Enviar datos por LoRa
void enviarDatosLoRa() {
    LoRa.beginPacket();
    LoRa.print("{\"tempAgua\":" + String(temperaturaAgua) + ",\"nivelAgua\":" + String(nivelAgua) + "}");
    LoRa.endPacket();
}

// Recibir datos LoRa
void recibirDatosLoRa() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String mensaje = "";
        while (LoRa.available()) mensaje += (char)LoRa.read();
        Serial.println("Mensaje recibido por LoRa: " + mensaje);
    }
}

// Enviar datos por BLE Mesh
void enviarDatosBLE() {
    // Código de transmisión BLE Mesh
}

// Recibir datos por BLE Mesh
void recibirDatosBLE() {
    // Código de recepción BLE Mesh
} 
