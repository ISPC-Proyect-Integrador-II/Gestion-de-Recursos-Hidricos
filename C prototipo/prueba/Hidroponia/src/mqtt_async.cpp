#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef USE_GSM
  #include "gsm_async.h"
  // Credenciales GPRS
  #define APN       "tu_apn"
  #define GPRS_USER ""
  #define GPRS_PASS ""
#else
  #include "wifi_async.h"
#endif
#include "mqtt_async.h"
#include <ArduinoJson.h>

#ifdef USE_GSM
  #include "gsm_async.h"
  #include <PubSubClient.h>
#else
  #include "wifi_async.h"
  #include <AsyncMqttClient.h>
  #include <freertos/FreeRTOS.h>
  #include <freertos/timers.h>
#endif

// ------------------------------------------------------------------
//                  Instancia del cliente MQTT
// ------------------------------------------------------------------
#ifdef USE_GSM
static PubSubClient mqttClient(gsmGetClient());
#else
static AsyncMqttClient    mqttClient;
static TimerHandle_t      mqttReconnectTimer;
static bool              _connected = false;
#endif

#ifndef USE_GSM
// Callbacks para AsyncMqttClient
static void onMqttConnect(bool sessionPresent) {
  Serial.println("MQTT conectado");
  _connected = true;
}

static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT desconectado, reintentando...");
  _connected = false;
  if (wifiIsConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

static void connectToMqtt(TimerHandle_t xTimer) {
  mqttClient.connect();
}
#endif

// ------------------------------------------------------------------
//                    API pública de mqtt_async
// ------------------------------------------------------------------

void mqttSetup(const char* server, uint16_t port) {
#ifdef USE_GSM
  mqttClient.setServer(server, port);
#else
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
#endif
}

void mqttStart() {
#ifdef USE_GSM
  if (gsmIsConnected() && !mqttClient.connected()) {
    mqttClient.connect("esp32-gsm");
  }
#else
  if (wifiIsConnected() && !_connected) {
    xTimerStart(mqttReconnectTimer, 0);
  }
#endif
}

void mqttLoop() {
#ifdef USE_GSM
  mqttClient.loop();
#else
#endif
}

void mqttSetCallback(MqttMessageCallback callback) {
#ifdef USE_GSM
  mqttClient.setCallback([callback]
    (char* topic, byte* payload, unsigned int length) {
      AsyncMqttClientMessageProperties props{};
      callback(topic, (char*)payload, props, length, 0, length);
    }
  );
#else
  mqttClient.onMessage(callback);
#endif
}

bool mqttPublishJson(const char* topic,
                     const JsonDocument& doc,
                     uint8_t qos,
                     bool retain) {
  char buf[256];
  size_t len = serializeJson(doc, buf, sizeof(buf));
#ifdef USE_GSM
  return mqttClient.publish(topic, buf, len);
#else
  auto packetId = mqttClient.publish(topic, qos, retain, buf, len);
  return packetId != 0;
#endif
}

bool mqttSubscribe(const char* topic, uint8_t qos) {
#ifdef USE_GSM
  return mqttClient.subscribe(topic, qos);
#else
  auto packetId = mqttClient.subscribe(topic, qos);
  return packetId != 0;
#endif
}
