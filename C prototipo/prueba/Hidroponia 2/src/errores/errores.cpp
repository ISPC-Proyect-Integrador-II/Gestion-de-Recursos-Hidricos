#include "errores.h"
#include <Arduino.h>

inline const char* getErrorMessage(ErrorCode code) {
  uint16_t idx = static_cast<uint16_t>(code);
  // leer desde PROGMEM
  return reinterpret_cast<const char*>(pgm_read_ptr(&_errMsgs[idx]));
}