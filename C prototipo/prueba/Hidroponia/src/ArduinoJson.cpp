/**
 * ArduinoJson.cpp - Implementación simplificada para proyecto de monitoreo ESP32
 */

 #include "ArduinoJson.h"

 namespace ArduinoJson {
 
 size_t serializeJson(const JsonDocument& doc, char* buffer, size_t size) {
     // Implementación simplificada que crea un JSON básico
     if (!buffer || size == 0) return 0;
     
     // Copiamos un JSON vacío por defecto
     const char* emptyJson = "{}";
     size_t len = strlen(emptyJson);
     
     if (len >= size) {
         len = size - 1;
     }
     
     memcpy(buffer, emptyJson, len);
     buffer[len] = '\0';
     
     return len;
 }
 
 size_t serializeJson(const JsonDocument& doc, Stream& output) {
     // Buffer temporal para serializar
     const size_t bufferSize = 256;
     char buffer[bufferSize];
     
     size_t len = serializeJson(doc, buffer, bufferSize);
     if (len > 0) {
         output.write(buffer, len);
     }
     
     return len;
 }
 
 size_t deserializeJson(JsonDocument& doc, const char* input) {
     // Implementación simplificada
     if (!input) return 0;
     return strlen(input);
 }
 
 size_t deserializeJson(JsonDocument& doc, Stream& input) {
     // Implementación básica
     if (!input.available()) return 0;
     
     // Leer hasta 256 bytes o hasta que no haya más datos
     char buffer[256];
     size_t len = 0;
     
     while (input.available() && len < 255) {
         buffer[len++] = input.read();
     }
     buffer[len] = '\0';
     
     return deserializeJson(doc, buffer);
 }
 
 } // namespace ArduinoJson