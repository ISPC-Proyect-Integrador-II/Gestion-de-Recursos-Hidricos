#ifndef SENSORES_H
#define SENSORES_H

#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// #include <Adafruit_INA219.h>
#include <adafruit_ina219_mock.h>

// Definiciones de pines para sensores
#define DHT_PIN 34
#define DHT_TYPE DHT11
#define DS18B20_PIN 25
#define FLOW_SENSOR_PIN 32
#define LDR_PIN 39
#define GAS_SENSOR_PIN 36
#define ULTRASONIC_TRIG_PIN 5
#define ULTRASONIC_ECHO_PIN 35
#define PH_SENSOR_PIN 2

// Variables globales para sensores
extern float temperaturaAgua;
extern float temperaturaAire;
extern float humedad;
extern float nivelAgua;
extern float flujoAgua;
extern int luz;
extern int gas;
extern float corriente;
extern float voltaje;
extern float potencia;
extern float phValor;

// Variables para activar/desactivar sensores dinámicamente
extern bool sensorTempAguaActivo;
extern bool sensorAireActivo;
extern bool sensorHumedadActivo;
extern bool sensorNivelAguaActivo;
extern bool sensorFlujoActivo;
extern bool sensorLuzActivo;
extern bool sensorGasActivo;
extern bool sensorCorrienteActivo;
extern bool sensorPHActivo;

// Inicialización de sensores
void inicializarSensores();

// Funciones de lectura con validación de activación/desactivación
bool leerTemperaturaAgua();
bool leerDHT();
bool leerNivelAgua();
bool calcularFlujoAgua();
bool leerLuz();
bool leerGas();
bool leerCorriente();
bool leerPH();

#endif