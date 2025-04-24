#include "mqtt_async.h"
#include "wifi_async.h"
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>     
#include <freertos/timers.h>       

static AsyncMqttClient mqttClient;
static TimerHandle_t   mqttReconnectTimer;
static bool            _connected = false;

// Cuando MQTT conecta
void onMqttConnect(bool sessionPresent) {
  Serial.println("MQTT conectado");
  _connected = true;
}

// Cuando MQTT se desconecta
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT desconectado, reintentando...");
  _connected = false;
  if (wifiIsConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

// Timer para reconectar
void connectToMqtt(TimerHandle_t xTimer) {
  mqttClient.connect();
}

void mqttSetup(const char* server, uint16_t port) {
  mqttReconnectTimer = xTimerCreate(
    "mqttTimer",
    pdMS_TO_TICKS(5000),
    pdFALSE,
    nullptr,
    connectToMqtt
  );

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.setServer(server, port);

  // Primer intento: solo si WiFi ya está conectado
  if (wifiIsConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void mqttLoop() {
  // Vacío: gestionado por callbacks y timer
}

void mqttSetCallback(MqttMessageCallback callback) {
  mqttClient.onMessage(callback);
}

bool mqttPublishJson(const char* topic,
                     const JsonDocument& doc,
                     uint8_t qos,
                     bool retain)
{
  char buf[256];
  size_t len = serializeJson(doc, buf, sizeof(buf));
  auto packetId = mqttClient.publish(topic, qos, retain, buf, len);
  return packetId != 0;
}

bool mqttSubscribe(const char* topic, uint8_t qos) {
  auto packetId = mqttClient.subscribe(topic, qos);
  return packetId != 0;
}
