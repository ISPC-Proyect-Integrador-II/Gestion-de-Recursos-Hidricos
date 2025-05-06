#include "sensores/ph_sensor.h"
#include <Arduino.h>
#include "sensores/sensores.h"


void inicializarSensorPH() {
  // No se requiere inicialización compleja, pero se puede calibrar aquí si es necesario
  Serial.println("Sensor de pH inicializado");
}

float leerSensorPH() {
  int lectura = analogRead(PH_SENSOR_PIN);
  // Conversión básica: ajustar la fórmula según la curva de tu sensor
  phValor = (lectura / 1024.0) * 14.0;
  return phValor;
} 
