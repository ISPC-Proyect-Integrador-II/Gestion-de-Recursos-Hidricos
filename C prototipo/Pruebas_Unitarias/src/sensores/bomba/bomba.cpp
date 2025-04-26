#include "bomba.h"

bomba::bomba(uint8_t PIN_BOMBA, int umbral)
  : PIN_BOMBA_(PIN_BOMBA)
  , umbral_(umbral)
  , estado_bomba_(false)
  , estado_timer_(false)
  , timerInicio_(0)
  , timerDuracion_(0)
{}

void bomba::iniciar() {
    pinMode(PIN_BOMBA_, OUTPUT);
    digitalWrite(PIN_BOMBA_, LOW);  // bomba apagada al inicio
}

void bomba::actualizar() {
    unsigned long now = millis();

    if (estado_timer_) {
        if (now - timerInicio_ >= timerDuracion_) {
            estado_timer_ = false;
            frenar_bomba();
        }
    } else {
        unsigned long fase = (now / 7000UL) % 2;
        int reading = (fase == 0)
                      ? umbral_ - 10  // 10 unidades por debajo
                      : umbral_ + 10; // 10 unidades por encima

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
