#include <Arduino.h>
#include "pantalla/pantalla.h"
#include "pantalla/menu.h"
#include "pantalla/menu_recetas.h"
#include "pantalla/menu_wifi.h"
#include "sensores/sensores.h"
#include "control/control.h"
#include "comunicacion/comunicacion.h"
#include "comunicacion/wifi_manager.h"
#include "comunicacion/gsm_async.h"
#include "rtc/rtc.h"
#include "recetas/recetas.h"
#include "transmisor.h"
#include "receptor.h"

//========================================================
// Variables para el intervalo de lectura
//========================================================

unsigned long ultimaLectura = 0;
const unsigned long intervaloLectura = 5000;

//========================================================
// Variable para obtener ID del nodo
//========================================================


//========================================================
// SETUP
//========================================================

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

//========================================================
// LLOP
//========================================================

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

    Serial.println("Hora actual: " + obtenerHora());
    ultimaLectura = millis();

    // Si es transmisor, envía datos por el canal seleccionado
    if (esNodoTransmisor) enviarDatosMQTT();
  }

  // LoRa en background
  if (loRaActivo) recibirDatosLoRa();

  delay(10);
}
