#ifndef ST7735_DRIVER_H
#define ST7735_DRIVER_H

#ifndef LV_CONF_INCLUDE_SIMPLE
  #define LV_CONF_INCLUDE_SIMPLE
#endif
#include "lv_conf.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

void initDisplay();
void handleDisplay();
void updateSensorLabels(float temperature, float humidity, float pressure);

#endif