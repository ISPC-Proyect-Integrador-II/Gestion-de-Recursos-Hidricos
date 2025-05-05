#include "ST7735.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

/// Ajusta a tu pantalla 128×160
static constexpr uint16_t SCREEN_WIDTH  = 128;
static constexpr uint16_t SCREEN_HEIGHT = 160;
static constexpr uint16_t BUFFER_LINES  = 40;

/// Instancia global de TFT_eSPI
static TFT_eSPI tft = TFT_eSPI();

/// Buffer estático de LVGL
static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf_1[SCREEN_WIDTH * BUFFER_LINES];

ST7735Display::ST7735Display() {}

void ST7735Display::begin() {
    // 1) LVGL init
    lv_init();

    // 2) TFT init
    tft.init();
    tft.setRotation(1);

    // 3) Draw buffer
    lv_disp_draw_buf_init(&disp_buf, buf_1, nullptr, SCREEN_WIDTH * BUFFER_LINES);

    // 4) Registrar driver
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = ST7735Display::disp_flush;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    lv_disp_drv_register(&disp_drv);

    // 5) Crear dos "pantallas"
    scr1 = lv_obj_create(nullptr);
    {
        auto* lbl = lv_label_create(scr1);
        lv_label_set_text(lbl, "Hello World!");
        lv_obj_center(lbl);
    }
    scr2 = lv_obj_create(nullptr);
    {
        auto* lbl = lv_label_create(scr2);
        lv_label_set_text(lbl, "Hola Mundo!");
        lv_obj_center(lbl);
    }

    // 6) Cargar la primera y medir el tiempo
    lv_scr_load(scr1);
    startTime = millis();
    transitioned = false;
}

void ST7735Display::update() {
    // 1) Procesa internals de LVGL
    lv_task_handler();

    // 2) Tras 10s, lanza la animación de barrido
    if (!transitioned && (millis() - startTime > 10000)) {
        transitioned = true;
        lv_scr_load_anim(
            scr2,
            LV_SCR_LOAD_ANIM_MOVE_LEFT,  // barrido izquierda
            500,                         // 500 ms duración
            0,                           // sin delay extra
            false                        // no invierte
        );
    }
}

/// Función de volcado de buffer a la pantalla
void ST7735Display::disp_flush(lv_disp_drv_t* disp,
                               const lv_area_t* area,
                               lv_color_t* color_p) {
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(reinterpret_cast<uint16_t*>(color_p), w * h, true);
    lv_disp_flush_ready(disp);
}
