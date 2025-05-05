#include <Arduino.h>
#include <ArduinoJson.h>

//====================================================================================
// INCLUDES/LIBRARIES
//====================================================================================
// de ser elementos relacionados a un FLAG llamar entre "#ifdef <FLAG>" y "#endif"
//====================================================================================


//==========================
// CONECTIVIDAD
//==========================
#ifdef USE_GSM
  #include "gsm_async.h"
  #define APN        "tu_apn"
  #define GPRS_USER  ""
  #define GPRS_PASS  ""
#else
  #include "wifi_async.h"
  #define SSID       "Vitto"
  #define PASS       "vittorio10"
#endif

#include "mqtt_async.h"

#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT   1883

//==========================
// SENSORES
//==========================

#ifdef SENSOR_SR04
  #include "sensores/sr04/sr04.h"
  #define TRIG_PIN 5
  #define ECHO_PIN 18
#endif

#ifdef SENSOR_BUZZER
  #include "sensores/buzzer/buzzer.h"
  #define BUZZER_PIN 5
#endif

//==========================
// ACTUADORES
//==========================

#ifdef ACTUADOR_BOMBA
  #include "sensores/bomba/bomba.h"
  #define BOMBA_PIN 32
  #define UMBRAL    50
  bomba bomba_1(BOMBA_PIN, UMBRAL);
#endif

//====================================================================================
// ESTRCUTRUA PARA MSJ MQTT
//====================================================================================
// En esto asignamos topics, formtato (json o strg) y QoS
//====================================================================================

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

//====================================================================================
// SETUP
//====================================================================================
// llamamos a los init de cada elemento, asi como asignar parametros basicos de incio
//====================================================================================

void setup() {
  Serial.begin(115200);

  //==========================
  // CONECTIVIDAD
  //==========================

  #ifdef USE_GSM
    gsmSetup(APN, GPRS_USER, GPRS_PASS);
  #else
    wifiSetup(SSID, PASS);
  #endif

  mqttSetup(MQTT_SERVER, MQTT_PORT);
  mqttSetCallback(onMqttMessage);

  //==========================
  // SENSORES
  //==========================
  #ifdef SENSOR_SR04
    sr04Begin(TRIG_PIN, ECHO_PIN);
  #endif

  #ifdef SENSOR_BUZZER
    buzzerInit(BUZZER_PIN);
  #endif

  //==========================
  // ACTUADORES
  //==========================
  #ifdef ACTUADOR_BOMBA
    bomba_1.iniciar();
    bomba_1.establecer_umbral(UMBRAL);
  #endif
}


//====================================================================================
// LOOP
//====================================================================================
// llamamos a las funciones que corresponden a cada elemento, y se ejecutan en el loop
//====================================================================================

void loop() {

  //==========================
  // CONECTIVIDAD
  //==========================  

  #ifdef USE_GSM
    gsmLoop();
  #else
    wifiLoop();
  #endif
  mqttLoop();


  //==========================
  // SENSORES
  //==========================
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

  //==========================
  // ACTUADORES
  //==========================
  #ifdef ACTUADOR_BOMBA
    static bool subscribed = false;
    if (!subscribed && netOK) {
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
      StaticJsonDocument<64> st2;
      st2["modo"] = currTimer ? "TEMPORIZADO" : "MANUAL";
      mqttPublishJson("actuador/bomba/estado", st2, 1, false);
    }

    lastOn    = currOn;
    lastTimer = currTimer;
  #endif




  delay(200);
}
