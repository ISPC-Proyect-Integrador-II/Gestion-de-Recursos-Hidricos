#include "control/iluminacion.h"
#include <Arduino.h>

void inicializarIluminacion() {
  pinMode(RELE_ILUMINACION_PIN, OUTPUT);
  digitalWrite(RELE_ILUMINACION_PIN, LOW);
}


