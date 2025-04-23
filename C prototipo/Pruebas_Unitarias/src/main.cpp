#include <Arduino.h>
#include "wifi_async.h"
#include "mqtt_async.h"
#include <ArduinoJson.h>

#define SSID        "Vitto"
#define PASS        "vittorio10"
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT   1883


// En este apartado llamamos a la flag/s que definimos en el .ini de esta forma solo se ejecuta la posicion de codigo que estamos probando en ese momento
#ifdef SENSOR_SR04
  #include "sensores/sr04/sr04.h" 
#endif

void onMqttMessage(char* topic,
                   char* payload,
                   AsyncMqttClientMessageProperties props,
                   size_t len,
                   size_t index,
                   size_t total) {
  Serial.printf("Recibido en %s: %.*s\n", topic, (int)len, payload);
}

void setup() {
  Serial.begin(115200);
  wifiSetup(SSID, PASS);
  mqttSetup(MQTT_SERVER, MQTT_PORT);
  mqttSetCallback(onMqttMessage);

  // Aca colocamos el iddef de la flag definida, y colocamos el setup correspondiente a ese.
  #ifdef SENSOR_SR04
    sr04Begin(TRIG_PIN, ECHO_PIN);
  #endif
}


void loop() {
  wifiLoop();
  mqttLoop();

  // cod de ejemplo para testear la conexion
  static unsigned long lastPub = 0;
  if (millis() - lastPub > 10000 && wifiIsConnected()) {
    lastPub = millis();
    StaticJsonDocument<128> doc;
    doc["temperatura"] = random(0, 40);
    doc["humedad"]     = random(20, 80);
    mqttPublishJson("sensores/data", doc, 1, false);
  }

  // Aca colocamos el iddef de la flag definida, y colocamos el loop correspondiente a ese.
  #ifdef SENSOR_SR04
    float d = sr04Read();

  #endiff
}