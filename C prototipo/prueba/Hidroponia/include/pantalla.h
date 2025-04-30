#ifndef PANTALLA_H
#define PANTALLA_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Definiciones de pines para TFT
#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2
#define TFT_MOSI   23
#define TFT_SCLK   18

// Definiciones para botones de navegación
#define BOTON_IZQUIERDA 32
#define BOTON_DERECHA   33
#define BOTON_CONFIRMAR 35

// Definiciones de animaciones
#define STANDBY_INTERVAL 5000    // Tiempo entre parpadeo de ojos
#define ALERTA_INTERVAL  500     // Tiempo entre parpadeo de alerta de agua

extern Adafruit_ST7735 tft;

void inicializarPantalla();
void actualizarPantalla();
void mostrarStandby();
void mostrarAlertaAgua();
void manejarBotones();
void mostrarDatos();
void mostrarMensaje(String mensaje);



#endif 
