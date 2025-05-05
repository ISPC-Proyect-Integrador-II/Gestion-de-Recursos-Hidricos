#include <Arduino.h>
#include <HardwareSerial.h>

// Definir pines del SIM800L
#define RX_PIN 16  // RX del ESP32 conectado a TX del SIM800L
#define TX_PIN 17  // TX del ESP32 conectado a RX del SIM800L

// Inicializar HardwareSerial en UART2
HardwareSerial sim800l(2);

void setup() {
    Serial.begin(9600);  // Configuración del Monitor Serie
    sim800l.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Comunicación con SIM800L

    Serial.println("🔍 Inicializando módulo SIM800L...");
    delay(3000);  // Esperar arranque del módulo

    // Prueba inicial con comando AT
    sim800l.println("AT");
}

void loop() {
    // Mostrar en el Monitor Serie la respuesta del SIM800L
    while (sim800l.available()) {
        Serial.write(sim800l.read());
    }

    // Permitir enviar comandos AT desde el Monitor Serie al SIM800L
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  // Leer comando del usuario
        sim800l.println(command);  // Enviar comando AT al SIM800L
    }

    delay(100);
}