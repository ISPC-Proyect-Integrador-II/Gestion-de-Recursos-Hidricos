#include "sr04.h"
#include <Arduino.h>

static uint8_t _pinTrig, _pinEcho;

void sro4Begin(uint8_t pinTrig, uint8_t pinEcho) {
    _pinTrig = pinTrig;
    _pinEcho = pinEcho;
    pinMode (pinTrig, OUTPUT);
    pinMode (pinEcho, INPUT);
    digitalWrite (_pinTrig = LOW)
}

void sr04Read() {
    unsigned long msActuales = millis();

    switch (sr04State){

        case SR04_IDLE:
            if (msActuales - sr04Timestamp >= INTERVALO_MEDICION){
                digitalWrite(_pinTrig, HIGH);
                sr04Timestamp = msActuales;
                sr04State = SR04_TRIGGER_PULSE;
            }
            break;
        
        case SR04_TRIGGER_PULSE:
            if (micros() - (sr04Timestamp*1000UL)>=10UL){
                digitalWrite(_pinTrig, LOW);
                sr04Timestamp = msActuales;
                sr04State = SR04_WAIT_HIGH;
            }
            break;
        
        case SR04_WAIT_HIGH:
            if (digitalRead(_pinEcho)==HIGH){
                pulseStartUs = micros();
                sr04State = SR04_WAIT_LOW;
            }
            
            else if (msActuales - sr04Timestamp >= TIMEOUT_ECHO){
                ultimaDistancia = -1;
                sr04Timestamp = msActuales;
                sro4State = SR04_IDLE
            } 
            break;
    }
}
