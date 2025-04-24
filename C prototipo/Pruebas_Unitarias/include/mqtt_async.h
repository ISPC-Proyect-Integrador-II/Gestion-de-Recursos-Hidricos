// include/mqtt_async.h
#ifndef MQTT_ASYNC_H
#define MQTT_ASYNC_H

#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <functional>               // <<-- Añade esto

// Definimos el alias con la firma que espera AsyncMqttClient::onMessage()
using MqttMessageCallback = std::function<
  void(char* topic,
       char* payload,
       AsyncMqttClientMessageProperties props,
       size_t len,
       size_t index,
       size_t total)
>;

/// Inicializa el cliente MQTT asíncrono
void mqttSetup(const char* server, uint16_t port);
/// Llamar en loop (vacío, callbacks/timer lo gestionan)
void mqttLoop();
/// Registra callback de mensaje
void mqttSetCallback(MqttMessageCallback callback);  // <<-- Aquí cambia el tipo
/// Publica un JsonDocument en "topic"
bool mqttPublishJson(const char* topic,
                     const JsonDocument& doc,
                     uint8_t qos = 0,
                     bool retain = false);
/// Suscribe a un topic
bool mqttSubscribe(const char* topic, uint8_t qos = 0);

#endif // MQTT_ASYNC_H
