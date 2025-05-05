#include <Arduino.h>
#include <HardwareSerial.h>

// Definir pines del SIM800L
#define RX_PIN 16  // RX del ESP32 conectado a TX del SIM800L
#define TX_PIN 17  // TX del ESP32 conectado a RX del SIM800L

// Inicializar HardwareSerial en UART2
HardwareSerial sim800l(2);

void setup() {
    Serial.begin(9600);  // Monitor Serie
    sim800l.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Configurar SIM800L en Serial2

    Serial.println("🔍 Inicializando módulo SIM800L...");
    delay(3000);  // Esperar arranque del SIM800L

    sim800l.println("AT");  // Enviar comando AT de prueba
}

void loop() {
    // Leer comandos AT ingresados en el Monitor Serie y enviarlos al SIM800L
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  // Leer comando del usuario
        sim800l.println(command);  // Enviar comando AT al SIM800L
    }

    // Mostrar respuesta del SIM800L en el Monitor Serie
    while (sim800l.available()) {
        Serial.write(sim800l.read());
    }

    delay(100);
}