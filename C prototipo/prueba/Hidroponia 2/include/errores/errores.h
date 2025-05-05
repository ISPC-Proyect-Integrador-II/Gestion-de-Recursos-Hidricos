#pragma once

#include <stdint.h>

// Nivel de severidad
enum class LogLevel : uint8_t {
  DEBUG, INFO, WARN, ERROR
};

// Códigos de error de tu sistema
enum class ErrorCode : uint16_t {
  NONE                = 0,
//================================================
// Errores de conectividad
//================================================
  WIFI_INIT_FAIL            = 1,        // error en inicializarWiFi();
  WIFI_AVAILABLE_NW_FAIL    = 2,        // error en escanearRedes()
  WIFI_CONNECT_FAIL         = 3,        // error en conectarWiFi()
  WIFI_CRED_SAVE_FAIL       = 4,        // error en guardarCredenciales

  LORA_INIT_FAIL            = 5,        // error en inicializarLoRaMesh()
  LORA_SEND_FAIL            = 6,        // error en enviarDatosLoRa()
  LORA_GET_FAIL             = 7,        // error en recibirDatosLoRa()

  GSM_INIT_FAIL             = 8,        // error en inicializarGsm()
  GSM_LOOP_FAIL             = 9,        // error en gsmLoop()
  GSM_STATE_FAIL            = 10,       // error en estadoGsm()
  GSM_MQTTClient_FAIL       = 11,       // error en gsmGetClient()
 
  BLE_INIT_FAIL             = 12,       // error en inicializarBLEMeshv
  BLE_SEND_FAIL             = 13,       // error en enviarDatosBLE
  BLE_GET_FAIL              = 14,       // error en recibirDatosBLE()

  MQTT_INIT_FAIL            = 15,       // error en conectarMQTT()
  MQTT_SEND_FAIL            = 16,       // error en enviarDatosMQTT()

//================================================
// Errores de Menu [R=recetas, W=WiFi, P=Principal]
//================================================

  RMENU_INIT_FAIL             = 17,       // error en mostrarMenuRecetas()
  RMENU_MANAGE_FAIL           = 18,       // error en manejarSeleccionReceta()

  WMENU_INIT_FAIL             = 19,       // error en mostrarMenuWiFi()
  WMENU_MANAGE_FAIL           = 20,       // error en manejarSeleccionWiFi()

  PMENU_INIT_FAIL             = 21,       // error en mostrarMenuPrincipal()
  PMENU_MANAGE_FAIL           = 22,       // error en procesarEntradaMenu()
  PMENU_SHOW_MSG_FAIL         = 23,       // error en mostrarMensaje()

//================================================
// Errores de Sensores
//================================================

  SENS_INIT_FAIL             = 24,       // error en inicializarSensores()
  SENS_TEMP_AGUA_FAIL        = 25,       // error en leerTemperaturaAgua()
  SENS_DTH_FAIL             = 26,       // error en leerDHT()
  SENS_NIVEL_AGUA_FAIL       = 27,       // error en leerNivelAgua()
  SENS_FLUJO_AGUA_FAIL       = 28,       // error en calcularFlujoAgua()
  SENS_LUZ_FAIL              = 29,       // error en leerLuz()
  SENS_GAS_FAIL              = 30,       // error en leerGas()
  SENS_CORRIENTE_FAIL        = 31,       // error en leerCorriente()
  SENS_PH_FAIL               = 32,       // error en leerPH()

//================================================
// Errores de Control
//================================================

  CONTROL_INIT_FAIL          = 33,      // error en inicializarControl()
  CONTROL_ACT_BOMBA_FAIL     = 34,      // error en activarBomba()
  CONTROL_ACT_LUCES_FAIL     = 35,      // error en activarIluminacion()
  CONTROL_VERIF_ALARM_FAIL   = 36,      // error en verificarAlarmas()
  CONTROL_NOTIF_ESTADO_FAIL  = 37,      // error en notificarEstado()
  CONTROL_AUTO_FAIL          = 38,      // error en controlAutomatico()

//================================================
// Errores de Pantalla
//================================================

  DISPLAY_INIT_FAIL          = 39,      // error en inicializarPantalla()
  DISPLAY_UPDATE_FAIL        = 40,      // error en actualizarPantalla()
  DISPLAY_SHOW_MSG_FAIL      = 41,      // error en mostrarMensaje()
  DISPLAY_SHOW_DATA_FAIL     = 42,      // error en mostrarDatos()
  DISPLAY_SHOW_STANDBY_FAIL  = 43,      // error en mostrarStandby()
  DISPLAY_SHOW_ALERTA_FAIL   = 44,      // error en mostrarAlertaAgua()
  DISPLAY_BOTONES_FAIL       = 45,      // error en manejarBotones()
  DISPLAY_DASHBOARD_FAIL     = 46,      // error en inicializarDashboard()

//================================================
// Errores de Recepcion
//================================================

  RECEPTOR_INIT_FAIL         = 47,      // error en inicializarReceptor()
  RECEPTOR_PROCESAR_FAIL     = 48,      // error en procesarDatos()

//================================================
// Errores de Recepcion
//================================================
  
    RTC_INIT_FAIL              = 49,      // error en inicializarRTC()
    RTC_GET_HORA_FAIL          = 50,      // error en obtenerHora()
    RTC_SET_HORA_FAIL          = 51,      // error en sincronizarHora()

//================================================
// Errores de Transmision
//================================================

  TRANSMISOR_INIT_FAIL       = 52,      // error en inicializarTransmisor()
  TRANSMISOR_ENVIAR_FAIL     = 53,      // error en enviarDatos()

//================================================
// Errores de Recetas
//================================================

  RECETAS_CARGAR_FAIL        = 54,      // error en cargarRecetas()
  RECETAS_GUARDAR_FAIL       = 55,      // error en guardarRecetas()
  RECETAS_SELECCIONAR_FAIL   = 56,      // error en seleccionarReceta()
  RECETAS_MOSTRAR_FAIL       = 57,      // error en mostrarRecetaActual()

};

//================================================
// Mapear código → índice en el array de strings
//================================================

static constexpr const char* const _errMsgs[] PROGMEM = {
  /*  0 */ "OK",
  // — Errores de conectividad WiFi
  /*  1 */ "Error al inicializar WiFi",
  /*  2 */ "Error al escanear redes WiFi",
  /*  3 */ "Error al conectar a la red WiFi",
  /*  4 */ "Error al guardar credenciales WiFi",
  // — Errores de LoRa
  /*  5 */ "Error al inicializar LoRa Mesh",
  /*  6 */ "Error al enviar datos por LoRa",
  /*  7 */ "Error al recibir datos por LoRa",
  // — Errores de GSM
  /*  8 */ "Error al inicializar módulo GSM",
  /*  9 */ "Error en bucle GSM",
  /* 10 */ "Estado GSM inválido",
  /* 11 */ "Error al obtener cliente MQTT sobre GSM",
  // — Errores de BLE
  /* 12 */ "Error al inicializar BLE Mesh",
  /* 13 */ "Error al enviar datos por BLE",
  /* 14 */ "Error al recibir datos por BLE",
  // — Errores de MQTT
  /* 15 */ "Error al inicializar conexión MQTT",
  /* 16 */ "Error al enviar datos por MQTT",
  // — Errores de Menú Recetas
  /* 17 */ "Error al inicializar menú de Recetas",
  /* 18 */ "Error al gestionar selección de receta",
  // — Errores de Menú WiFi
  /* 19 */ "Error al inicializar menú de WiFi",
  /* 20 */ "Error al gestionar selección de WiFi",
  // — Errores de Menú Principal
  /* 21 */ "Error al inicializar menú Principal",
  /* 22 */ "Error al procesar entrada del menú",
  /* 23 */ "Error al mostrar mensaje en menú Principal",
  // — Errores de Sensores
  /* 24 */ "Error al inicializar sensores",
  /* 25 */ "Error al leer temperatura del agua",
  /* 26 */ "Error al leer sensor DHT",
  /* 27 */ "Error al leer nivel de agua",
  /* 28 */ "Error al calcular flujo de agua",
  /* 29 */ "Error al leer intensidad de luz",
  /* 30 */ "Error al leer sensor de gas",
  /* 31 */ "Error al leer sensor de corriente",
  /* 32 */ "Error al leer sensor de pH",
  // — Errores de Control
  /* 33 */ "Error al inicializar módulo de Control",
  /* 34 */ "Error al activar bomba",
  /* 35 */ "Error al activar iluminación",
  /* 36 */ "Error al verificar alarmas",
  /* 37 */ "Error al notificar estado",
  /* 38 */ "Error en rutina de control automático",
  // — Errores de Pantalla
  /* 39 */ "Error al inicializar pantalla",
  /* 40 */ "Error al actualizar pantalla",
  /* 41 */ "Error al mostrar mensaje en pantalla",
  /* 42 */ "Error al mostrar datos en pantalla",
  /* 43 */ "Error al mostrar modo standby",
  /* 44 */ "Error al mostrar alerta de agua",
  /* 45 */ "Error al manejar botones de la pantalla",
  /* 46 */ "Error al inicializar dashboard en pantalla",
  // — Errores de Receptor
  /* 47 */ "Error al inicializar receptor",
  /* 48 */ "Error al procesar datos del receptor",
  // — Errores de RTC
  /* 49 */ "Error al inicializar RTC",
  /* 50 */ "Error al obtener hora del RTC",
  /* 51 */ "Error al sincronizar hora del RTC",
  // — Errores de Transmisor
  /* 52 */ "Error al inicializar transmisor",
  /* 53 */ "Error al enviar datos con el transmisor",
  // — Errores de Recetas
  /* 54 */ "Error al cargar recetas",
  /* 55 */ "Error al guardar recetas",
  /* 56 */ "Error al seleccionar receta",
  /* 57 */ "Error al mostrar receta actual"
};

