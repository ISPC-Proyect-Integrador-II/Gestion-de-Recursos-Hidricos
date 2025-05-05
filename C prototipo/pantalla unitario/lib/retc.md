#include <Wire.h>
#include <RTClib.h>

// Definir los pines de conexión del RTC DS1307
#define SDA_PIN 21   // Pin de datos I2C
#define SCL_PIN 22   // Pin de reloj I2C

// Inicializar el RTC
RTC_DS1307 rtc;

void setup() {
    Serial.begin(9600);
    Serial.println("Test unitario - RTC DS1307");

    // Configurar pines I2C
    Wire.begin(SDA_PIN, SCL_PIN);

    // Verificar si el RTC está conectado
    if (!rtc.begin()) {
        Serial.println(" Error: No se encontró el módulo RTC.");
        while (1); // Detener ejecución si no hay RTC
    }

    // Comprobar si el RTC tiene una hora válida
    if (!rtc.isrunning()) {
        Serial.println(" RTC sin hora válida. Configurando hora actual...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Establece la fecha y hora del código actual
    }
}

void loop() {
    // Leer la hora actual
    DateTime now = rtc.now();

    Serial.print("🕒 Fecha y hora: ");
    Serial.print(now.year());
    Serial.print("-");
    Serial.print(now.month());
    Serial.print("-");
    Serial.print(now.day());
    Serial.print(" ");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());

    delay(1000); // Actualizar cada segundo
}