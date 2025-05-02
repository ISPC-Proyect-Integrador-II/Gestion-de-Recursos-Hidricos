#include "rtc.h"

RTC_DS1307 rtc;
DateTime horaActual;

// Inicialización del RTC
void inicializarRTC() {
    if (!rtc.begin()) {
        Serial.println("Error al iniciar RTC");
        return;
    }

    if (!rtc.isrunning()) {
        Serial.println("RTC no tiene hora válida, configurando...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

// Obtener hora actual
String obtenerHora() {
    horaActual = rtc.now();
    return String(horaActual.year()) + "-" + 
           String(horaActual.month()) + "-" + 
           String(horaActual.day()) + " " + 
           String(horaActual.hour()) + ":" + 
           String(horaActual.minute());
}

// Sincronizar hora desde MQTT
void sincronizarHora(String nuevaHora) {
    int year, month, day, hour, minute;
    sscanf(nuevaHora.c_str(), "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute);
    rtc.adjust(DateTime(year, month, day, hour, minute, 0));
    Serial.println("Hora sincronizada desde MQTT: " + nuevaHora);
} 
