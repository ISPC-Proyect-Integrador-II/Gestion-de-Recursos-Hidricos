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

void onMqttMessage(char* topic,
                   char* payload,
                   AsyncMqttClientMessageProperties props,
                   size_t len,
                   size_t index,
                   size_t total) {
  Serial.printf("Recibido en %s: %.*s\n",
                topic, int(len), payload);
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
    buzzerInit(buzzerPin);
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
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      cmd.trim();
      if      (cmd == "100") buzzerSetLevel(A_LOW);
      else if (cmd == "200") buzzerSetLevel(A_MEDIUM);
      else if (cmd == "300") buzzerSetLevel(A_HIGH);
      else                   buzzerSetLevel(A_NONE);
    }
    buzzerUpdate();
  #endif 
}
