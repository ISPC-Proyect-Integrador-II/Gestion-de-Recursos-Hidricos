#include <Arduino.h>
#include <ArduinoJson.h>
#include "wifi_async.h"
#include "mqtt_async.h"

#define SSID        "Vitto"
#define PASS        "vittorio10"
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT   1883

#ifdef SENSOR_SR04
  #include "sensores/sr04/sr04.h"
  #define TRIG_PIN 5
  #define ECHO_PIN 18
#endif

#ifdef SENSOR_BUZZER
    #include "sensores/buzzer/buzzer.h"
    #define BUZZER_PIN 5
#endif

#ifdef ACTUADOR_BOMBA
  #include "sensores/bomba/bomba.h"
  #define BOMBA_PIN 32
  #define UMBRAL    50
  bomba bomba_1(BOMBA_PIN, UMBRAL);
#endi


void onMqttMessage(char* topic,
                   char* payload,
                   AsyncMqttClientMessageProperties props,
                   size_t len,
                   size_t index,
                   size_t total) {
  Serial.printf("Recibido en %s: %.*s\n",
                topic, int(len), payload);

  #ifdef ACTUADOR_BOMBA
    if (String(topic) == "casa/bomba/control") {
    bomba_1.comando(String(payload).substring(0, len));
    }
  #endif
                
}


void setup() {
  Serial.begin(115200);
  wifiSetup(SSID, PASS);
  mqttSetup(MQTT_SERVER, MQTT_PORT);
  mqttSetCallback(onMqttMessage);

  #ifdef SENSOR_SR04
    sr04Begin(TRIG_PIN, ECHO_PIN);
  #endif

  #ifdef SENSOR_BUZZER
    buzzerInit(BUZZER_PIN);
  #endif

  #ifdef ACTUADOR_BOMBA
    bomba_1.iniciar();
    bomba_1.establecer_umbral(UMBRAL);
  #endif
}


void loop() {
  wifiLoop();
  mqttLoop();

  // Ejemplo de publicación periódica
  static unsigned long lastPub = 0;
  if (millis() - lastPub > 10000 && wifiIsConnected()) {
    lastPub = millis();
    StaticJsonDocument<128> doc;
    doc["temperatura"] = random(0, 40);
    doc["humedad"]     = random(20, 80);
    mqttPublishJson("sensores/data-prueba", doc, 1, false);
  }

  #ifdef SENSOR_SR04
    float distancia = sr04Read();
    if (distancia >= 0) {
      Serial.printf("Distancia SR04: %.1f cm\n", distancia);
      // Publicacion MQTT - Topic sensores/sr04
        StaticJsonDocument<64> j;
        j["distancia"] = distancia;
        mqttPublishJson("sensores/sr04", j, 1, false);
    } else {
      Serial.println("Lectura SR04 inválida");
    }
  #endif
  
  #ifdef SENSOR_BUZZER
  if      (cmd == "100") { 
    currentAlarm = A_LOW; 
    Serial.println("Alarma nivel bajo activada"); 
  }
  else if (cmd == "200") { 
    currentAlarm = A_MEDIUM; 
    Serial.println("Alarma nivel medio activada"); 
  }
  else if (cmd == "300") { 
    currentAlarm = A_HIGH; 
    Serial.println("Alarma nivel alto activada"); 
  }
  else { 
    currentAlarm = A_NONE; 
    Serial.println("Alarma desactivada");
  }
  buzzerUpdate();
  #endif

  #ifdef ACTUADOR_BOMBA
    bomba_1.actualizar();

    // Usa los getters en vez de acceder a miembros privados
    Serial.println(bomba_1.isOn() ? "Bomba encendida" : "Bomba apagada");
    Serial.println(bomba_1.isTimerActive() 
                   ? "Bomba en modo temporizado" 
                   : "Bomba en modo manual");
  #endif

  delay(100);
}
