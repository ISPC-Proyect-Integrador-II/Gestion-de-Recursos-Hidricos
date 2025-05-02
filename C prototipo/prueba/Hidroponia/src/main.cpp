#include <Arduino.h>
#include "pantalla.h"
#include "sensores.h"
#include "control.h"
#include "comunicacion.h"
#include "wifi_manager.h"
#include "rtc.h"
#include "recetas.h"
#include "menu.h"
#include "menu_recetas.h"
#include "menu_wifi.h"
#include "transmisor.h"
#include "receptor.h"
#include "gsm_async.h"

// Variables para el intervalo de lectura
unsigned long ultimaLectura = 0;
const unsigned long intervaloLectura = 5000;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando sistema hidroponico...");

  inicializarPantalla();
  inicializarSensores();
  inicializarControl();

  // 1) Ejecutamos la inicialización unificada de canales:
  inicializarComunicaciones();

  // 2) Si Wi-Fi está activo, intentamos conectar
  if (wifiActivo) {
    conectarWiFi();
    conectarMQTT_WiFi();    // Conecta MQTT sólo por Wi-Fi al inicio
  }

  // 3) Transmisor/Receptor, RTC y Recetas como antes
  inicializarRTC();
  cargarRecetas();
  mostrarRecetaActual();
  inicializarTransmisor();
  inicializarReceptor();

  Serial.println("Sistema listo.");
}

void loop() {
  mostrarMenuPrincipal();
  manejarSeleccionMenu();
  actualizarPantalla();

  // Mantiene vivas las comunicaciones y procesa MQTT
  manejarComunicacion();

  // Cada X ms: lee sensores activos, controla y envía
  if (millis() - ultimaLectura >= intervaloLectura) {
    if (sensorTempAguaActivo)  leerTemperaturaAgua();
    if (sensorAireActivo)      leerDHT();
    if (sensorNivelAguaActivo) leerNivelAgua();
    if (sensorFlujoActivo)     calcularFlujoAgua();
    if (sensorLuzActivo)       leerLuz();
    if (sensorGasActivo)       leerGas();
    if (sensorCorrienteActivo) leerCorriente();
    if (sensorPHActivo)        leerPH();

    controlAutomatico();
    verificarAlarmas();

    // Si es transmisor, envía datos por el canal seleccionado
    if (esNodoTransmisor) enviarDatosMQTT();

    Serial.println("Hora actual: " + obtenerHora());
    ultimaLectura = millis();
  }

  // LoRa en background
  if (loRaActivo) recibirDatosLoRa();

  delay(10);
}
