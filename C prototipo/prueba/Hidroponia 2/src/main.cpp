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
extern bool pantallaNecesitaActualizacion;  // Declaración global


void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Iniciando sistema hidroponico...");

 // declaracion spi
  SPI.begin(TFT_CLK, TFT_MISO, TFT_MOSI, TFT_CS);
  // bienvenida
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);

  // **Dibujar suelo más abajo**
  tft.fillRect(0, 150, 128, 10, BROWN);

  // **Animación de crecimiento de la planta más ancha**
  for (int altura = 5; altura <= 80; altura += 3) {
      tft.fillRect(55, 140 - altura, 18, altura, DARK_GREEN); // Tallo base más ancho
      tft.fillRect(55, 140 - altura, 18, altura - 4, LIGHT_GREEN); // Resalte del tallo

      // **Hojas ovaladas simuladas con círculos y rectángulo**
      if (altura > 15) {
          tft.fillCircle(45, 140 - altura, 10, LIGHT_GREEN); // Lado izquierdo
          tft.fillCircle(75, 140 - altura, 10, LIGHT_GREEN); // Lado derecho
          tft.fillRect(45, 140 - altura - 5, 30, 10, LIGHT_GREEN); // Centro de la hoja
      }
      if (altura > 30) {
          tft.fillCircle(40, 140 - altura - 10, 12, DARK_GREEN);
          tft.fillCircle(80, 140 - altura - 10, 12, DARK_GREEN);
          tft.fillRect(40, 140 - altura - 15, 40, 12, DARK_GREEN);
      }
      if (altura > 50) {
          tft.fillCircle(55, 140 - altura - 15, 14, LIGHT_GREEN);
          tft.fillCircle(73, 140 - altura - 15, 14, LIGHT_GREEN);
          tft.fillRect(55, 140 - altura - 20, 18, 14, LIGHT_GREEN);
      }

      delay(250);
  }

  // **Flor más grande en la cima**
  tft.fillCircle(64, 40, 8, RED);      // Pétalos
  tft.fillCircle(64, 40, 6, ORANGE);   // Detalle intermedio
  tft.fillCircle(64, 40, 4, YELLOW);   // Centro de la flor

  // **Mantener el logo visible unos segundos**
  delay(5000);

  inicializarDashboard();

  //mostrarMenuPrincipal(); // Mostrar el menú después del mensaje de bienvenida

  // Inicializar comunicación serial para depuración
  

  // Inicialización de la pantalla
 /// inicializarPantalla();

  // Inicialización de sensores
  inicializarSensores();

  // Inicialización del módulo de control automático
  //inicializarControl();

  // Inicialización de WiFi y MQTT (solo si está activado)
  //inicializarWiFi();
  //if (wifiActivo) conectarWiFi();
  ///if (wifiActivo) conectarMQTT();

  // Inicialización de otros canales de comunicación
  //if (loRaActivo) conectarLoRa();      // LoRa Mesh
  //if (bleActivo) conectarBLEMesh();   // BLE Mesh

  // Inicialización del reloj RTC
  //inicializarRTC();

  // Cargar recetas de cultivo almacenadas en EEPROM y mostrar la receta por defecto
 // cargarRecetas();
  //mostrarRecetaActual();

  // Inicializar modos de Transmisor y Receptor
 // inicializarTransmisor();
  //inicializarReceptor();

  // Configurar pines adicionales (si es necesario)
  Serial.println("Sistema listo.");
}

void loop() {
  // Actualiza la pantalla y maneja los botones físicos
  //manejarSeleccionMenu();

  inicializarDashboard();

    inicializarDashboard();
 /*    
 if (!menuVisible) {  // Si el menú no está activo, volver a mostrarlo
        mostrarMenuPrincipal();
    }



  if (pantallaNecesitaActualizacion) {
      mostrarMenuPrincipal();  // Redibuja el menú principal
      pantallaNecesitaActualizacion = false;  // Evita redibujar innecesariamente
  }
/* 
  // Mantiene activa la comunicación MQTT (si está habilitada)
  //if (wifiActivo) manejarMQTT();

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

    /* // Se ejecuta la automatización: controla la bomba, la iluminación y verifica alarmas
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

 /**/ // Pequeño delay para estabilidad en el loop
  delay(10);
}