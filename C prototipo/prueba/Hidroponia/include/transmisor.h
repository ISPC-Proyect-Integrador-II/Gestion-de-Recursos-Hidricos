// transmisor.h
#ifndef TRANSMISOR_H
#define TRANSMISOR_H

#include <Arduino.h>            // String
#include <ArduinoJson.h>        // StaticJsonDocument
#include <WiFi.h>               // WiFi.macAddress()
#include "sensores/sensores.h"   // variables de lectura (temperaturaAgua, phValor, etc.) :contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}
#include "control/control.h"     // funciones estadoBomba(), estadoLuces(), alarmaActiva(), modoAutomatico()
#include "rtc/rtc.h"             // obtenerHora()
#include "comunicacion/comunicacion.h" // cliente MQTT “client” y flags wifiActivo/forceGsm

extern bool esNodoTransmisor;  // definido en transmisor.cpp

/// Inicialización en modo transmisor (usado desde main.cpp)
void inicializarTransmisor();

/// Lee las variables globales, arma el JSON y lo publica por MQTT
void enviarDatos();

#endif
