#ifndef st7735_h
#define st7735_h

#include <lvgl.h>


// Inicializa pantalla y LVGL, crea pantallas y temporizador
void initDisplay();

// Debe llamarse en loop() para procesar LVGL
void handleDisplay();

// Actualiza las etiquetas del dashboard con nuevas lecturas
void updateSensorLabels(float temperature, float humidity, float pressure);
#endif