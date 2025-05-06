#include "receptor.h"
#include "comunicacion/comunicacion.h"
#include "rtc/rtc.h"
#include <ArduinoJson.h>

//=======================================================
// Marca este ESP32 como receptor para acumular lotes
//=======================================================

bool esNodoReceptor = true;

//=======================================================
// Documento global para el batch
//=======================================================
static StaticJsonDocument<1024> batchDoc;
static JsonArray nodes = batchDoc.createNestedArray("nodes");

/// Callback MQTT para todos los mensajes
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (!esNodoReceptor) return;
  if (strcmp(topic, "hidroponia/datos") != 0) return;

  // Copiamos payload a cadena nula-terminada
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0';

  // Procesamos el nodo entrante
  procesarDatos(msg);
}

//=======================================================
// INICIALIZACIÓN DEL RECEPTOR
//=======================================================

void inicializarReceptor() {
  Serial.println("Modo: RECEPTOR inicializado");

  // 1) Preparamos el JSON global con gatewayId
  batchDoc["gatewayId"] = String(WiFi.macAddress());

  // 2) Registramos el callback y nos suscribimos al topic de datos
  wifiMqttClient.setCallback(mqttCallback);
  gsmMqttClient.setCallback(mqttCallback);
  wifiMqttClient.subscribe("hidroponia/datos");
  gsmMqttClient.subscribe("hidroponia/datos");
}

//=======================================================
// PROCESAMIENTO DE NODOS
//=======================================================

void procesarDatos(const char* mensaje) {
  Serial.println("Nodo recibido:");
  Serial.println(mensaje);

  StaticJsonDocument<512> nodoDoc;
  auto err = deserializeJson(nodoDoc, mensaje);
  if (err) {
    Serial.println("Error al parsear JSON del nodo");
    return;
  }

  // Añadimos el objeto completo al array "nodes"
  nodes.add(nodoDoc.as<JsonVariant>());
}

/// Llama a esta función periódicamente (por ejemplo cada X segundos o después de N nodos)
void publicarBatch() {
  if (!esNodoReceptor) return;

  // 1) Actualizamos timestamp global
  batchDoc["timestamp"] = obtenerHora();  // p.ej. "2025-05-05T18:00:00Z"

  // 2) Serializamos y publicamos
  char buf[1024];
  size_t len = serializeJson(batchDoc, buf);
  if (wifiActivo && wifiMqttClient.connected()) {
    wifiMqttClient.publish("hidroponia/batch", buf, len);
  } else if (gsmMqttClient.connected()) {
    gsmMqttClient.publish("hidroponia/batch", buf, len);
  }

  // 3) Limpiamos para la siguiente ronda
  nodes.clear();
}
