
 #ifndef ARDUINO_JSON_H
 #define ARDUINO_JSON_H
 
 #include <Arduino.h>
 #include <string.h>
 #include <stdint.h>
 
 namespace ArduinoJson {
 
 // Definiciones previas
 class JsonObject;
 class JsonArray;
 class JsonVariant;
 
 // Clase para el documento JSON
 class JsonDocument {
 private:
     static const size_t DEFAULT_CAPACITY = 256;
     char* _buffer;
     size_t _capacity;
     size_t _size;
     
 public:
     JsonDocument(size_t capacity = DEFAULT_CAPACITY) : _capacity(capacity), _size(0) {
         _buffer = new char[capacity];
         _buffer[0] = '\0';
     }
     
     ~JsonDocument() {
         if (_buffer) delete[] _buffer;
     }
     
     // No permitir copias para evitar problemas de memoria
     JsonDocument(const JsonDocument&) = delete;
     JsonDocument& operator=(const JsonDocument&) = delete;
     
     // Permitir mover
     JsonDocument(JsonDocument&& other) noexcept : _buffer(other._buffer), _capacity(other._capacity), _size(other._size) {
         other._buffer = nullptr;
         other._capacity = 0;
         other._size = 0;
     }
     
     JsonDocument& operator=(JsonDocument&& other) noexcept {
         if (this != &other) {
             if (_buffer) delete[] _buffer;
             _buffer = other._buffer;
             _capacity = other._capacity;
             _size = other._size;
             other._buffer = nullptr;
             other._capacity = 0;
             other._size = 0;
         }
         return *this;
     }
     
     JsonObject createNestedObject(const char* key);
     JsonArray createNestedArray(const char* key);
     
     template<typename T>
     bool set(const char* key, T value) {
         char valueStr[64];
         if (std::is_same<T, float>::value || std::is_same<T, double>::value) {
             dtostrf(static_cast<float>(value), 6, 2, valueStr);
         } else if (std::is_same<T, int>::value || std::is_same<T, long>::value) {
             sprintf(valueStr, "%ld", static_cast<long>(value));
         } else if (std::is_same<T, bool>::value) {
             sprintf(valueStr, "%s", value ? "true" : "false");
         } else {
             // Para strings y otros tipos
             strncpy(valueStr, static_cast<const char*>(value), sizeof(valueStr) - 1);
             valueStr[sizeof(valueStr) - 1] = '\0';
         }
         
         return true;
     }
     
     void clear() {
         _size = 0;
         if (_buffer) _buffer[0] = '\0';
     }
     
     size_t capacity() const { return _capacity; }
     size_t size() const { return _size; }
     
     // Para serialización interna
     char* getBuffer() { return _buffer; }
     
     JsonObject as();
 };
 
 // Definición de StaticJsonDocument
 template<size_t CAPACITY>
 class StaticJsonDocument : public JsonDocument {
 public:
     StaticJsonDocument() : JsonDocument(CAPACITY) {}
 };
 
 // Definición de DynamicJsonDocument
 class DynamicJsonDocument : public JsonDocument {
 public:
     explicit DynamicJsonDocument(size_t capacity) : JsonDocument(capacity) {}
 };
 
 // Clase para objetos JSON
 class JsonObject {
 private:
     JsonDocument* _doc;
     
 public:
     JsonObject() : _doc(nullptr) {}
     explicit JsonObject(JsonDocument* doc) : _doc(doc) {}
     
     template<typename T>
     bool set(const char* key, T value) {
         if (!_doc) return false;
         return _doc->set(key, value);
     }
     
     JsonObject createNestedObject(const char* key);
     JsonArray createNestedArray(const char* key);
     
     bool isNull() const { return _doc == nullptr; }
 };
 
 // Clase para arrays JSON
 class JsonArray {
 private:
     JsonDocument* _doc;
     int _index;
     
 public:
     JsonArray() : _doc(nullptr), _index(0) {}
     explicit JsonArray(JsonDocument* doc) : _doc(doc), _index(0) {}
     
     template<typename T>
     bool add(T value) {
         if (!_doc) return false;
         // Implementación simplificada
         _index++;
         return true;
     }
     
     JsonObject createNestedObject();
     JsonArray createNestedArray();
     
     bool isNull() const { return _doc == nullptr; }
 };
 
 // Implementación de métodos
 inline JsonObject JsonDocument::as() {
     return JsonObject(this);
 }
 
 inline JsonObject JsonDocument::createNestedObject(const char* key) {
     return JsonObject(this);
 }
 
 inline JsonArray JsonDocument::createNestedArray(const char* key) {
     return JsonArray(this);
 }
 
 inline JsonObject JsonArray::createNestedObject() {
     return JsonObject(_doc);
 }
 
 inline JsonArray JsonArray::createNestedArray() {
     return JsonArray(_doc);
 }
 
 inline JsonObject JsonObject::createNestedObject(const char* key) {
     return JsonObject(_doc);
 }
 
 inline JsonArray JsonObject::createNestedArray(const char* key) {
     return JsonArray(_doc);
 }
 
 // Funciones para serializar/deserializar
 size_t serializeJson(const JsonDocument& doc, char* buffer, size_t size);
 size_t serializeJson(const JsonDocument& doc, Stream& output);
 size_t deserializeJson(JsonDocument& doc, const char* input);
 size_t deserializeJson(JsonDocument& doc, Stream& input);
 
 } // namespace ArduinoJson
 
 // Para compatibilidad con código existente
 using namespace ArduinoJson;
 
 #endif // ARDUINO_JSON_H