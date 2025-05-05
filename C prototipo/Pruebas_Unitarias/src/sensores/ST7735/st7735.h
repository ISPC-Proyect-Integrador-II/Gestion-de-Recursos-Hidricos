#ifndef ST7735DISPLAY_H
#define ST7735DISPLAY_H

#define LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#include <lvgl.h>

class ST7735Display {
public:
    ST7735Display();
    /// Inicializa LVGL y la pantalla
    void begin();
    /// Debe llamarse en loop() para procesar LVGL y la transición
    void update();

private:
    lv_obj_t* scr1 = nullptr;
    lv_obj_t* scr2 = nullptr;
    uint32_t startTime = 0;
    bool transitioned = false;

    /// Callback que LVGL usa para volcar el buffer a la pantalla
    static void disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);
};

#endif // ST7735DISPLAY_H
