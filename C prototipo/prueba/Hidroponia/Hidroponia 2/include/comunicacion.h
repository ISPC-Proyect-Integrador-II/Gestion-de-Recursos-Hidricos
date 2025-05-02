#ifndef COMUNICACION_H
#define COMUNICACION_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <LoRa.h>
#include <BLEDevice.h>
// Variables para activar/desactivar comunicación
extern bool wifiActivo;
extern bool loRaActivo;
extern bool bleActivo;

// Definiciones de conexión WiFi
#define WIFI_SSID "TU_SSID"
#define WIFI_PASSWORD "TU_PASSWORD"

// Configuración MQTT
#define MQTT_BROKER "broker.mqtt.com"
#define MQTT_PORT 1883
#define MQTT_USER "usuario"
#define MQTT_PASSWORD "password"
#define CLIENT_ID "ESP32_Hidroponico"

// Definiciones LoRa
#define LORA_CS 15
#define LORA_RST 13
#define LORA_DIO0 34
#define LORA_FREQUENCY 915E6  // Ajustar según región

// Variables globales
extern WiFiClient espClient;
extern PubSubClient client;

// Inicialización de comunicación
void conectarWiFi();
void conectarMQTT();
void conectarLoRa();
void conectarBLEMesh();

// Funciones de transmisión y recepción
void manejarMQTT();
void enviarDatosMQTT();
void enviarDatosLoRa();
void recibirDatosLoRa();
void enviarDatosBLE();
void recibirDatosBLE();

#endif 
