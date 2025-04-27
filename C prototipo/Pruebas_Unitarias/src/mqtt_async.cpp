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

// ------------------------------------------------------------------
//               Callbacks y reconexión (solo Wi-Fi)
// ------------------------------------------------------------------
#ifndef USE_GSM
// Cuando MQTT conecta (AsyncMqttClient)
static void onMqttConnect(bool sessionPresent) {
  Serial.println("MQTT conectado");
  _connected = true;
}

// Cuando MQTT se desconecta
static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT desconectado, reintentando...");
  _connected = false;
  if (wifiIsConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

// Timer para reconectar
static void connectToMqtt(TimerHandle_t xTimer) {
  mqttClient.connect();
}
#endif

// ------------------------------------------------------------------
//                    API pública de mqtt_async
// ------------------------------------------------------------------

void mqttSetup(const char* server, uint16_t port) {
#ifdef USE_GSM
  // Configura PubSubClient sobre GPRS
  mqttClient.setServer(server, port);
#else
  // Configura AsyncMqttClient sobre Wi-Fi
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
  // Conexión síncrona en PubSubClient
  if (gsmIsConnected() && !mqttClient.connected()) {
    mqttClient.connect();
  }
#else
  // Desencadena reconexión asíncrona
  if (wifiIsConnected() && !_connected) {
    xTimerStart(mqttReconnectTimer, 0);
  }
#endif
}

void mqttLoop() {
#ifdef USE_GSM
  // Mantiene vivo el cliente PubSubClient
  mqttClient.loop();
#else
  // AsyncMqttClient usa timers/callbacks
#endif
}

void mqttSetCallback(MqttMessageCallback callback) {
#ifdef USE_GSM
  // Adaptador de firma: PubSubClient → MqttMessageCallback
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
  // Publicación en PubSubClient
  return mqttClient.publish(topic, buf, len);
#else
  // Publicación en AsyncMqttClient
  auto packetId = mqttClient.publish(topic, qos, retain, buf, len);
  return packetId != 0;
#endif
}

bool mqttSubscribe(const char* topic, uint8_t qos) {
#ifdef USE_GSM
  // Suscripción PubSubClient
  return mqttClient.subscribe(topic, qos);
#else
  // Suscripción AsyncMqttClient
  auto packetId = mqttClient.subscribe(topic, qos);
  return packetId != 0;
#endif
}
