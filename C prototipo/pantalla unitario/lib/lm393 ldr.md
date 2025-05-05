
#include <Arduino.h>
// Definir el pin digital del sensor de luz
#define LDR_DIGITAL_PIN 35  // Usa el mismo pin que en tu código

void setup() {
    Serial.begin(9600);
    Serial.println("Test unitario - Sensor de luz LM393");

    pinMode(LDR_DIGITAL_PIN, INPUT); // Configurar como entrada digital
}

void loop() {
    Serial.println("Leyendo estado de luz...");

    int estadoLuz = digitalRead(LDR_DIGITAL_PIN); // Leer la salida digital

    if (estadoLuz == LOW) {
        Serial.println("Luz detectada (brillante)");
    } else {
        Serial.println("Oscuridad detectada (poca luz)");
    }

    delay(500); // Esperar antes de la próxima medición
}