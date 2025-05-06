#include "errores/errores.h"
#include <Arduino.h>

inline const char* getErrorMessage(ErrorCode code) {
  uint16_t idx = static_cast<uint16_t>(code);
  // leer desde PROGMEM
  return reinterpret_cast<const char*>(pgm_read_ptr(&_errMsgs[idx]));
}

void registrarError(ErrorCode code) {
  // 1) Tu lógica actual de guardar en flash...
  //    (puede ser un write a EEPROM o SPIFFS, lo que uses)

  // 2) Construir JSON y enviarlo por Serial
  const char* msg = getErrorMessage(code);
  char jsonBuf[128];
  snprintf(jsonBuf, sizeof(jsonBuf),
      "{\"timestamp\":%lu,\"error_code\":%u,\"message\":\"%s\"}",
      millis(),                              // timestamp
      static_cast<uint16_t>(code),           // código numérico
      msg);                                  // texto descriptivo
  Serial.println(jsonBuf);                  // sale algo como:
                                            // {"timestamp":123456,"error_code":32,"message":"Error al leer sensor de pH"}
}