#include "DHTesp.h"

// Declaración de pines
#define DHT_PIN 21  // Usa el mismo pin que en tu código

// Inicialización del sensor
DHTesp dhtSensor;

void setup() {
    Serial.begin(9600);
    Serial.println("Test unitario - Sensor DHT11");
    
    dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
}

void loop() {
    Serial.println("Leyendo temperatura y humedad...");
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    if (isnan(data.temperature) || isnan(data.humidity)) {
        Serial.println("Error: Fallo al leer DHT.");
    } else {
        Serial.print("Temperatura del aire: ");
        Serial.print(data.temperature);
        Serial.print(" °C - Humedad: ");
        Serial.print(data.humidity);
        Serial.println(" %");
    }
    
    delay(2000); // Esperar 2 segundos antes de la próxima lectura
}