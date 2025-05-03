#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef USE_GSM
  #include "gsm_async.h"
  #define APN       "tu_apn"
  #define GPRS_USER ""
  #define GPRS_PASS ""
#else
  #include "wifi_async.h"
#endif
#include "mqtt_async.h"

#ifndef USE_GSM
  #define SSID        "Vitto"
  #define PASS        "vittorio10"
#endif
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT   1883

#ifdef SENSOR_SR04
  #include "sensores/sr04/sr04.h"
  #define TRIG_PIN 5
  #define ECHO_PIN 18
#endif

#ifdef SENSOR_BUZZER
  #include "sensores/buzzer/buzzer.h"
  #define BUZZER_PIN 5
#endif

#ifdef ACTUADOR_BOMBA
  #include "sensores/bomba/bomba.h"
  #define BOMBA_PIN 32
  #define UMBRAL    50
  bomba bomba_1(BOMBA_PIN, UMBRAL);
#endif

#ifdef ST7735
  #include "sensores/ST7735/ST7735.h"
  #define TFT_DC 12
  #define TFT_CS 13
  #define TFT_MOSI 14
  #define TFT_CLK 27
  #define TFT_RST 0
  #define TFT_MISO 0
#endif


void onMqttMessage(char* topic,
                   char* payload,
                   AsyncMqttClientMessageProperties props,
                   size_t len,
                   size_t index,
                   size_t total) {
  String msg = String(payload).substring(0, len);

  #ifdef ACTUADOR_BOMBA
  if (String(topic) == "casa/bomba/control") {
    bomba_1.comando(msg);
    Serial.printf("Comando → %s\n", msg.c_str());
  }
  #endif
}

void setup() {
  Serial.begin(115200);
#ifdef USE_GSM
  gsmSetup(APN, GPRS_USER, GPRS_PASS);
#else
  wifiSetup(SSID, PASS);
#endif

  mqttSetup(MQTT_SERVER, MQTT_PORT);
  mqttSetCallback(onMqttMessage);

  #ifdef SENSOR_SR04
    sr04Begin(TRIG_PIN, ECHO_PIN);
  #endif

  #ifdef SENSOR_BUZZER
    buzzerInit(BUZZER_PIN);
  #endif

  #ifdef ACTUADOR_BOMBA
    bomba_1.iniciar();
    bomba_1.establecer_umbral(UMBRAL);
  #endif

  #ifdef ST7735
  
  void initDisplay()

  #endif
}

void loop() {
#ifdef USE_GSM
  gsmLoop();
#else
  wifiLoop();
#endif
  mqttLoop();

  static unsigned long lastPub = 0;
#ifdef USE_GSM
  if (millis() - lastPub > 10000 && gsmIsConnected()) {
#else
  if (millis() - lastPub > 10000 && wifiIsConnected()) {
#endif
    lastPub = millis();
    StaticJsonDocument<128> doc;
    doc["temperatura"] = random(0, 40);
    doc["humedad"]     = random(20, 80);
    mqttPublishJson("sensores/data-prueba", doc, 1, false);
  }

  #ifdef SENSOR_SR04
    float distancia = sr04Read();
    if (distancia >= 0) {
      Serial.printf("Distancia SR04: %.1f cm\n", distancia);
      StaticJsonDocument<64> j;
      j["distancia"] = distancia;
      mqttPublishJson("sensores/sr04", j, 1, false);
    } else {
      Serial.println("Lectura SR04 inválida");
    }
  #endif

  #ifdef SENSOR_BUZZER
    buzzerUpdate();
  #endif

  #ifdef ACTUADOR_BOMBA
    static bool subscribed = false;
#ifdef USE_GSM
    if (!subscribed && gsmIsConnected()) {
#else
    if (!subscribed && wifiIsConnected()) {
#endif
      mqttSubscribe("casa/bomba/control", 0);
      subscribed = true;
    }

    bomba_1.actualizar();

    static bool lastOn    = bomba_1.isOn();
    static bool lastTimer = bomba_1.isTimerActive();

    bool currOn    = bomba_1.isOn();
    bool currTimer = bomba_1.isTimerActive();

    if (currOn != lastOn) {
      Serial.println(currOn ? ">>> Bomba ENCENDIDA" : ">>> Bomba APAGADA");
      StaticJsonDocument<64> st;
      st["estado"] = currOn ? "ON" : "OFF";
      mqttPublishJson("actuador/bomba/estado", st, 1, false);
    }

    if (currTimer != lastTimer) {
      Serial.println(currTimer
        ? ">>> MODO TEMPORIZADO ACTIVADO"
        : ">>> MODO MANUAL");
      StaticJsonDocument<64> st;
      st["modo"] = currTimer ? "TEMPORIZADO" : "MANUAL";
      mqttPublishJson("actuador/bomba/estado", st, 1, false);
    }

    lastOn    = currOn;
    lastTimer = currTimer;
  #endif

  #ifdef ST7735
  void handleDisplay();
  #endif

  delay(200);
}
