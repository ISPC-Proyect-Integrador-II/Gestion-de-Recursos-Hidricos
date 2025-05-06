#ifndef COMUNICACION_H
#define COMUNICACION_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <LoRa.h>
#include <BLEDevice.h>
#include "gsm_async.h"
#include "wifi_manager.h"     // Para usar conectarWiFi(ssid, pass)

// Flags globales de comunicación
extern bool wifiActivo;
extern bool loRaActivo;
extern bool bleActivo;
extern bool forceGsm;        // Forzar GSM en lugar de Wi-Fi

// Credenciales MQTT (se mantienen comunes)
#define MQTT_BROKER    "broker.mqtt.com"
#define MQTT_PORT      1883
#define MQTT_USER      "usuario"
#define MQTT_PASSWORD  "password"
#define CLIENT_ID      "ESP32_Hidroponico"

// Definiciones LoRa
#define LORA_CS        15
#define LORA_RST       13
#define LORA_DIO0      34
#define LORA_FREQUENCY 915E6

// Definiciones GSM (ya vienen de gsm_async.h)
#define APN            "tu.apn"
#define GPRS_USER      "user"
#define GPRS_PASS      "pass"

// Cliente MQTT Wi-Fi y GSM
extern WiFiClient        espClient;
extern PubSubClient      wifiMqttClient;
extern PubSubClient      gsmMqttClient;

// Inicialización de comunicaciones
void inicializarComunicaciones();      // Inicia Wi-Fi, GSM, LoRa, BLE
void conectarWiFi();                   // Antes standalone
void conectarMQTT_WiFi();              // Antes standalone
void conectarGSM_MQTT();               // Antes standalone
void conectarLoRa();
void conectarBLEMesh();

// Función unificada para inicializar el canal MQTT (escoge Wi-Fi o GSM)
void conectarMQTT();                   // ¡Nuevo!

// Manejo y envío de datos
void manejarComunicacion();            // Llama a MQTT/LoRa/BLE según flags y estado
void enviarDatosMQTT();                // Publica por Wi-Fi o GSM según estado
void enviarDatosLoRa();
void recibirDatosLoRa();
void enviarDatosBLE();
void recibirDatosBLE();

#endif // COMUNICACION_H
