#include <Arduino.h>
// Definir el pin del caudalímetro
#define FLOW_SENSOR_PIN 32  // Ajustado al pin correcto

// Variables para el conteo de pulsos
volatile int pulsosFlujo = 0;
float factorCalibracion = 4.5;  // Ajustar según el modelo de sensor

// Función de interrupción para contar pulsos del flujo de agua
void IRAM_ATTR contadorPulsos() {
    pulsosFlujo++;
}

void setup() {
    Serial.begin(9600);
    Serial.println("Test unitario - Sensor de flujo de agua");

    // Configurar el pin del sensor como entrada con pull-up interno
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);

    // Asignar la interrupción al pin de flujo
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), contadorPulsos, FALLING);
}

void loop() {
    Serial.println("Midiendo flujo de agua...");

    // Convertir pulsos en litros por minuto
    float flujoAgua = pulsosFlujo * factorCalibracion;

    Serial.print("Flujo de agua: ");
    Serial.print(flujoAgua);
    Serial.println(" L/min");

    // Reiniciar el conteo para la siguiente medición
    pulsosFlujo = 0;

    delay(2000); // Esperar antes de la próxima lectura
}