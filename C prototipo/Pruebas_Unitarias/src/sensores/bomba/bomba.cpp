#include "bomba.h"

bomba::bomba(uint8_t PIN_BOMBA, int umbral)
  : PIN_BOMBA_(PIN_BOMBA),
    umbral_(umbral),
    estado_bomba_(false),
    estado_timer_(false),
    timerInicio_(0),
    timerDuracion_(0)
{}

void bomba::iniciar() {
    pinMode(PIN_BOMBA_, OUTPUT);
    digitalWrite(PIN_BOMBA_, LOW);  // bomba apagada inicialmente
}

void bomba::actualizar() {
    unsigned long now = millis();

    if (estado_timer_) {
        // Si el temporizador expiró, parar la bomba y desactivar timer
        if (now - timerInicio_ >= timerDuracion_) {
            estado_timer_ = false;
            frenar_bomba();
        }
    } else {
        // Lectura simulada de sensor (reemplazar por analogRead(sensorPin_))
        int reading = 1000; 
        if (reading >= umbral_) {
            if (!estado_bomba_) encender_bomba();
        } else {
            if (estado_bomba_) frenar_bomba();
        }
    }
}

void bomba::comando(const String& msg) {
    String m = msg;
    m.trim();

    if (m.equalsIgnoreCase("ON")) {
        estado_timer_ = false;
        encender_bomba();
    }
    else if (m.equalsIgnoreCase("OFF")) {
        estado_timer_ = false;
        frenar_bomba();
    }
    else {
        unsigned long secs = m.toInt();
        if (secs > 0) {
            encender_bomba();
            estado_timer_   = true;
            timerInicio_    = millis();
            timerDuracion_  = secs * 1000UL;
        }
    }
}

void bomba::establecer_umbral(int umbral) {
    umbral_ = umbral;
}

bool bomba::isOn() const {
    return estado_bomba_;
}

bool bomba::isTimerActive() const {
    return estado_timer_;
}

void bomba::encender_bomba() {
    digitalWrite(PIN_BOMBA_, HIGH);
    estado_bomba_ = true;
}

void bomba::frenar_bomba() {
    digitalWrite(PIN_BOMBA_, LOW);
    estado_bomba_ = false;
}
