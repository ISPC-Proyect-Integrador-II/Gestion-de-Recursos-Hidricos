#pragma once
#include "errores.h"
#include <Arduino.h>

#ifndef LOG_LEVEL
  #define LOG_LEVEL LogLevel::DEBUG
#endif

class Logger {
public:
  static void begin(unsigned long baud=115200) {
    Serial.begin(baud);
    while(!Serial) { delay(10); }
  }

  template<typename... Args>
  static void log(LogLevel lvl, const char* fmt, Args... args) {
    if (lvl < LOG_LEVEL) return;
    const char* tag = levelTag(lvl);
    Serial.printf("[%s] ", tag);
    Serial.printf(fmt, args...);
    Serial.println();
  }

  static void logError(ErrorCode code) {
    if (LogLevel::ERROR < LOG_LEVEL) return;
    const char* msg = getErrorMessage(code);
    Serial.printf("[ERR] (%u) %s\n", static_cast<uint16_t>(code), msg);
  }

private:
  static const char* levelTag(LogLevel lvl) {
    switch(lvl) {
      case LogLevel::DEBUG: return "DBG";
      case LogLevel::INFO:  return "INF";
      case LogLevel::WARN:  return "WRN";
      case LogLevel::ERROR: return "ERR";
    }
    return "";
  }
};
