#include "st7735.h"
#include <TFT_eSPI.h>
#include <Arduino.h>

// Resolución de pantalla
static constexpr uint16_t HOR_RES = 128;
static constexpr uint16_t VER_RES = 160;

// Objeto TFT
static TFT_eSPI tft;

// LVGL buffers y driver
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t      disp_drv;

// Objetos de pantalla y widgets
static lv_timer_t * screen_timer;
static lv_obj_t *  menu_screen;
static lv_obj_t *  dashboard_screen;
static lv_obj_t *  label_temp;
static lv_obj_t *  label_humidity;
static lv_obj_t *  label_pressure;

// Callback de flushing: copia un bloque de píxeles a la TFT
static void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
  tft.startWrite();
  for (int y = area->y1; y <= area->y2; y++) {
    tft.setAddrWindow(area->x1, y, area->x2 - area->x1 + 1, 1);
    // Empuja 'width' píxeles de una sola fila
    tft.pushColors(&color_p->full, area->x2 - area->x1 + 1, true);
    color_p += area->x2 - area->x1 + 1;
  }
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

// Temporizador para alternar entre menú y dashboard
static void switch_screen(lv_timer_t * timer) {
  static bool show_menu = true;
  if (show_menu) {
    lv_scr_load_anim(dashboard_screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
  } else {
    lv_scr_load_anim(menu_screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, false);
  }
  show_menu = !show_menu;
}

void initDisplay() {
  // Inicializa TFT
  tft.init();
  tft.setRotation(1);

  // Inicializa LVGL
  lv_init();

  // Buffer de dibujo: 10 líneas de alto
  static lv_color_t buf[HOR_RES * 10];
  lv_disp_draw_buf_init(&draw_buf, buf, nullptr, HOR_RES * 10);

  // Configura y registra el driver de display
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = HOR_RES;
  disp_drv.ver_res = VER_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Construye la pantalla de menú
  menu_screen = lv_obj_create(nullptr);
  {
    lv_obj_t * lbl = lv_label_create(menu_screen);
    lv_label_set_text(lbl, "Menú Principal");
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 10);

    for (int i = 0; i < 3; i++) {
      lv_obj_t * btn = lv_btn_create(menu_screen);
      lv_obj_set_size(btn, 100, 40);
      lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10, 40 + 50 * i);
      lv_obj_t * l2 = lv_label_create(btn);
      lv_label_set_text(l2, (i == 0) ? "Opción 1" : (i == 1) ? "Opción 2" : "Opción 3");
      lv_obj_center(l2);
    }
  }

  // Construye la pantalla de dashboard
  dashboard_screen = lv_obj_create(nullptr);
  {
    lv_obj_t * lbl = lv_label_create(dashboard_screen);
    lv_label_set_text(lbl, "Dashboard de Sensores");
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 10);

    label_temp     = lv_label_create(dashboard_screen);
    label_humidity = lv_label_create(dashboard_screen);
    label_pressure = lv_label_create(dashboard_screen);

    lv_label_set_text(label_temp,     "Temperatura: --°C");
    lv_label_set_text(label_humidity, "Humedad: --%");
    lv_label_set_text(label_pressure, "Presión: ---- hPa");

    lv_obj_align(label_temp,     LV_ALIGN_LEFT_MID, 10, -30);
    lv_obj_align(label_humidity, LV_ALIGN_LEFT_MID, 10,   0);
    lv_obj_align(label_pressure, LV_ALIGN_LEFT_MID, 10,  30);
  }

  // Carga inicialmente el menú
  lv_scr_load(menu_screen);

  // Crea el timer que alterna cada 5s
  screen_timer = lv_timer_create(switch_screen, 5000, nullptr);
}

void handleDisplay() {
  lv_timer_handler();
}

void updateSensorLabels(float temperature, float humidity, float pressure) {
  static char bufStr[32];
  snprintf(bufStr, sizeof(bufStr), "Temperatura: %.1f°C", temperature);
  lv_label_set_text(label_temp, bufStr);

  snprintf(bufStr, sizeof(bufStr), "Humedad: %.1f%%", humidity);
  lv_label_set_text(label_humidity, bufStr);

  snprintf(bufStr, sizeof(bufStr), "Presión: %.0f hPa", pressure);
  lv_label_set_text(label_pressure, bufStr);
}
