#ifndef MQTT_ASYNC_H
#define MQTT_ASYNC_H

#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

/// Inicializa el cliente MQTT asíncrono
void mqttSetup(const char* server, uint16_t port);
/// Llamar en loop (vacío, callbacks/timer lo gestionan)
void mqttLoop();
/// Registra callback de mensaje
void mqttSetCallback(AsyncMqttClientMessageHandler callback);
/// Publica un JsonDocument en "topic"
bool mqttPublishJson(const char* topic,
                     const JsonDocument& doc,
                     uint8_t qos = 0,
                     bool retain = false);
/// Suscribe a un topic
bool mqttSubscribe(const char* topic, uint8_t qos = 0);

#endif // MQTT_ASYNC_H