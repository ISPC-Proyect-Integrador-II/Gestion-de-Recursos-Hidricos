#include "sr04.h"
#include <Arduino.h>

// Máquina de estados
enum Sr04State : uint8_t {
  SR04_IDLE,
  SR04_TRIGGER_PULSE,
  SR04_WAIT_HIGH,
  SR04_WAIT_LOW
};

static Sr04State      _state         = SR04_IDLE;
static unsigned long _lastMs        = 0;     // marca de tiempo en ms
static unsigned long _pulseStartUs  = 0;     // marca de tiempo en µs
static float         _lastDistance  = -1;    // distancia calculada

static uint8_t       _pinTrig;
static uint8_t       _pinEcho;

// Parámetros (ajustables)
static const unsigned long INTERVALO_MEDICION = 200UL;   // ms entre mediciones
static const unsigned long TIMEOUT_ECHO       = 30UL;    // ms para ver HIGH
static const unsigned long TIMEOUT_PULSO      = 30000UL; // µs para ver LOW

void sr04Begin(uint8_t pinTrig, uint8_t pinEcho) {
  _pinTrig       = pinTrig;
  _pinEcho       = pinEcho;
  pinMode(_pinTrig, OUTPUT);
  pinMode(_pinEcho, INPUT);
  digitalWrite(_pinTrig, LOW);

  _state         = SR04_IDLE;
  _lastMs        = millis();
  _lastDistance  = -1;
}

float sr04Read() {
  unsigned long nowMs = millis();

  switch (_state) {
    case SR04_IDLE:
      // ¿Toca iniciar nueva medición?
      if (nowMs - _lastMs >= INTERVALO_MEDICION) {
        digitalWrite(_pinTrig, HIGH);
        _lastMs = nowMs;
        _state  = SR04_TRIGGER_PULSE;
      }
      break;

    case SR04_TRIGGER_PULSE:
      // Tras 10 µs, bajamos TRIG y pasamos a esperar eco
      if (micros() - (_lastMs * 1000UL) >= 10UL) {
        digitalWrite(_pinTrig, LOW);
        _lastMs = nowMs;
        _state  = SR04_WAIT_HIGH;
      }
      break;

    case SR04_WAIT_HIGH:
      // Llega el pulso de eco: arrancamos temporizador en µs
      if (digitalRead(_pinEcho) == HIGH) {
        _pulseStartUs = micros();
        _state        = SR04_WAIT_LOW;
      }
      // Timeout si no llega eco en TIMEOUT_ECHO ms
      else if (nowMs - _lastMs >= TIMEOUT_ECHO) {
        _lastDistance = -1;
        _lastMs       = nowMs;
        _state        = SR04_IDLE;
      }
      break;

    case SR04_WAIT_LOW:
      // Fin del eco: calculamos duración y distancia
      if (digitalRead(_pinEcho) == LOW) {
        unsigned long durationUs = micros() - _pulseStartUs;
        _lastDistance = (durationUs * 0.0343f) / 2.0f;
        _lastMs       = nowMs;
        _state        = SR04_IDLE;
      }
      // Timeout si nunca baja en TIMEOUT_PULSO µs
      else if (micros() - _pulseStartUs >= TIMEOUT_PULSO) {
        _lastDistance = -1;
        _lastMs       = nowMs;
        _state        = SR04_IDLE;
      }
      break;
  }

  return _lastDistance;
}
