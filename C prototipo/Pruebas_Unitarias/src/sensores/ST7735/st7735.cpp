#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>



// Resolución LCD
static const uint16_t HOR_RES = 128;
static const uint16_t VER_RES = 160;

// Objetos y drivers LVGL/TFT
static TFT_eSPI tft = TFT_eSPI();
static lv_disp_drv_t disp_drv;
static lv_obj_t * menu_screen;
static lv_obj_t * dashboard_screen;
static lv_timer_t * screen_timer;
static lv_obj_t * label_temp;
static lv_obj_t * label_humidity;
static lv_obj_t * label_pressure;

// Flush callback: dibuja píxel a píxel en TFT
static void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    tft.startWrite();
    for (int y = area->y1; y <= area->y2; y++) {
        for (int x = area->x1; x <= area->x2; x++) {
            tft.drawPixel(x, y, color_p->full);
            color_p++;
        }
    }
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

// Alterna entre menú y dashboard cada 5s
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
    // Inicializar TFT
    tft.init();
    tft.setRotation(1);

    // Inicializar LVGL
    lv_init();
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[HOR_RES * 10];
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, HOR_RES * 10);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = HOR_RES;
    disp_drv.ver_res = VER_RES;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Crear pantalla de menú
    menu_screen = lv_obj_create(NULL);
    lv_obj_t * label_menu = lv_label_create(menu_screen);
    lv_label_set_text(label_menu, "Menú Principal");
    lv_obj_align(label_menu, LV_ALIGN_TOP_MID, 0, 10);

    for (int i = 0; i < 3; i++) {
        lv_obj_t * btn = lv_btn_create(menu_screen);
        lv_obj_set_pos(btn, 10, 40 + i*50);
        lv_obj_set_size(btn, 100, 40);
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, (i==0?"Opción 1":i==1?"Opción 2":"Opción 3"));
    }

    // Crear pantalla de dashboard
    dashboard_screen = lv_obj_create(NULL);
    lv_obj_t * label_dash = lv_label_create(dashboard_screen);
    lv_label_set_text(label_dash, "Dashboard de Sensores");
    lv_obj_align(label_dash, LV_ALIGN_TOP_MID, 0, 10);

    label_temp     = lv_label_create(dashboard_screen);
    label_humidity = lv_label_create(dashboard_screen);
    label_pressure = lv_label_create(dashboard_screen);

    lv_label_set_text(label_temp,     "Temperatura: --°C");
    lv_label_set_text(label_humidity, "Humedad: --%");
    lv_label_set_text(label_pressure, "Presión: ---- hPa");

    lv_obj_align(label_temp,     LV_ALIGN_LEFT_MID, 10, -30);
    lv_obj_align(label_humidity, LV_ALIGN_LEFT_MID, 10,   0);
    lv_obj_align(label_pressure, LV_ALIGN_LEFT_MID, 10,  30);

    // Cargar menú inicialmente y crear temporizador
    lv_scr_load(menu_screen);
    screen_timer = lv_timer_create(switch_screen, 5000, NULL);
}

void handleDisplay() {
    lv_timer_handler();
    delay(5);
}

void updateSensorLabels(float temperature, float humidity, float pressure) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Temperatura: %.1f°C", temperature);
    lv_label_set_text(label_temp, buf);
    snprintf(buf, sizeof(buf), "Humedad: %.1f%%", humidity);
    lv_label_set_text(label_humidity, buf);
    snprintf(buf, sizeof(buf), "Presión: %.0f hPa", pressure);
    lv_label_set_text(label_pressure, buf);
}