#include "sensores/sensores.h"

// Objetos para sensores
DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
Adafruit_INA219 ina219;

// Variables de sensores
float temperaturaAgua = 0.0;
float temperaturaAire = 0.0;
float humedad = 0.0;
float nivelAgua = 0.0;
float flujoAgua = 0.0;
int luz = 0;
int gas = 0;
float corriente = 0.0;
float voltaje = 0.0;
float potencia = 0.0;
float phValor = 7.0;

// Variables para activar/desactivar sensores
bool sensorTempAguaActivo = true;
bool sensorAireActivo = true;
bool sensorHumedadActivo = true;
bool sensorNivelAguaActivo = true;
bool sensorFlujoActivo = true;
bool sensorLuzActivo = true;
bool sensorGasActivo = true;
bool sensorCorrienteActivo = true;
bool sensorPHActivo = true;

// Inicialización de sensores
void inicializarSensores() {
    dht.begin();
    sensors.begin();
    ina219.begin();
}

// Sensores con validación de activación/desactivación

bool leerTemperaturaAgua() {
    if (!sensorTempAguaActivo) return false;
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    if (temp < -50.0) return false;  // Sensor desconectado
    temperaturaAgua = temp;
    return true;
}

bool leerDHT() {
    if (!sensorAireActivo || !sensorHumedadActivo) return false;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) return false; // Error en lectura
    humedad = h;
    temperaturaAire = t;
    return true;
}

bool leerNivelAgua() {
    if (!sensorNivelAguaActivo) return false;
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

    long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
    nivelAgua = (duration * 0.034) / 2;
    return nivelAgua > 0;
}

bool calcularFlujoAgua() {
    if (!sensorFlujoActivo) return false;
    flujoAgua = analogRead(FLOW_SENSOR_PIN) / 7.5;  // Ajustar según calibración
    return flujoAgua > 0;
}

bool leerLuz() {
    if (!sensorLuzActivo) return false;
    luz = analogRead(LDR_PIN);
    return true;
}

bool leerGas() {
    if (!sensorGasActivo) return false;
    gas = analogRead(GAS_SENSOR_PIN);
    return gas > 0;
}

bool leerCorriente() {
    if (!sensorCorrienteActivo) return false;
    corriente = ina219.getCurrent_mA() / 1000.0;
    voltaje = ina219.getBusVoltage_V();
    potencia = corriente * voltaje;
    return corriente >= 0;
}

bool leerPH() {
    if (!sensorPHActivo) return false;
    int lectura = analogRead(PH_SENSOR_PIN);
    phValor = (lectura / 1024.0) * 14.0;  // Ajustar calibración
    return phValor > 0;
}