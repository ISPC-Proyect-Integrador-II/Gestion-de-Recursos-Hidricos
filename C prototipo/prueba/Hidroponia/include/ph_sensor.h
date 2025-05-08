#ifndef PH_SENSOR_H
#define PH_SENSOR_H

// Definición del pin del sensor de pH y constantes de calibración
#define PH_SENSOR_PIN 35

extern float phValor;

void inicializarSensorPH();
float leerSensorPH();

#endif 
