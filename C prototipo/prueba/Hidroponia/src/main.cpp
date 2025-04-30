// Incluir las librerías y módulos desarrollados
#include <Arduino.h>
#include "pantalla.h"
#include "sensores.h"
#include "control.h"
#include "comunicacion.h"
#include "wifi_manager.h"
#include "rtc.h"
#include "recetas.h"
#include "menu.h"           // Menú principal, si es necesario
#include "menu_recetas.h"   // Para seleccionar recetas de cultivo
#include "menu_wifi.h"      // Para selección de red WiFi
#include "transmisor.h"     // Modo transmisor
#include "receptor.h"       // Modo receptor

// Variables globales para el intervalo de lectura
unsigned long ultimaLectura = 0;
const unsigned long intervaloLectura = 5000;  // Intervalo de lectura en milisegundos (ej. 5 segundos)

void setup() {
  // Inicializar comunicación serial para depuración
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando sistema hidroponico...");

  // Inicialización de la pantalla
  inicializarPantalla();

  // Inicialización de sensores
  inicializarSensores();

  // Inicialización del módulo de control automático
  inicializarControl();

  // Inicialización de WiFi y MQTT (solo si está activado)
  inicializarWiFi();
  if (wifiActivo) conectarWiFi();
  if (wifiActivo) conectarMQTT();

  // Inicialización de otros canales de comunicación
  if (loRaActivo) conectarLoRa();      // LoRa Mesh
  if (bleActivo) conectarBLEMesh();   // BLE Mesh

  // Inicialización del reloj RTC
  inicializarRTC();

  // Cargar recetas de cultivo almacenadas en EEPROM y mostrar la receta por defecto
  cargarRecetas();
  mostrarRecetaActual();

  // Inicializar modos de Transmisor y Receptor
  inicializarTransmisor();
  inicializarReceptor();

  // Configurar pines adicionales (si es necesario)
  Serial.println("Sistema listo.");
}

void loop() {
  // Actualiza la pantalla y maneja los botones físicos
  mostrarMenuPrincipal();  
  manejarSeleccionMenu();
  actualizarPantalla();

  // Mantiene activa la comunicación MQTT (si está habilitada)
  if (wifiActivo) manejarMQTT();

  // Cada intervaloLectura, se leen los sensores y se ejecuta la lógica de control
  if (millis() - ultimaLectura >= intervaloLectura) {
    // **Solo se leen los sensores activados**
    if (sensorTempAguaActivo) leerTemperaturaAgua();
    if (sensorAireActivo) leerDHT();
    if (sensorNivelAguaActivo) leerNivelAgua();
    if (sensorFlujoActivo) calcularFlujoAgua();
    if (sensorLuzActivo) leerLuz();
    if (sensorGasActivo) leerGas();
    if (sensorCorrienteActivo) leerCorriente();
    if (sensorPHActivo) leerPH();

    // Se ejecuta la automatización: controla la bomba, la iluminación y verifica alarmas
    controlAutomatico();
    verificarAlarmas();

    // Si este nodo está configurado como transmisor y WiFi está activo, envía los datos
    if (esNodoTransmisor && wifiActivo) {
      enviarDatos();
    }

    // Actualiza el reloj y muestra la hora actual en el Serial Monitor
    String horaActualStr = obtenerHora();
    Serial.println("Hora actual: " + horaActualStr);

    // Reinicia el contador de tiempo
    ultimaLectura = millis();
  }

  // Procesa mensajes entrantes desde LoRa (solo si está activado)
  if (loRaActivo) recibirDatosLoRa();

  // Pequeño delay para estabilidad en el loop
  delay(10);
}