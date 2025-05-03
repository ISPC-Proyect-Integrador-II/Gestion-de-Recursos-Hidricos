#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   Graphical settings
 *====================*/

/* Max horizontal and vertical resolution */
#define LV_HOR_RES_MAX          (128)
#define LV_VER_RES_MAX          (160)

/* Color depth: 1, 8, 16 or 32 bit per pixel */
#define LV_COLOR_DEPTH          16
/* Swap the two bytes of RGB565 color if your display driver requires it */
#define LV_COLOR_16_SWAP        0

/*------------------
 *  Memory settings
 *-----------------*/

/* Size of the memory available for LVGL (in bytes). Adjust if you add more widgets */
#define LV_MEM_SIZE             (32U * 1024U)

/* Use built-in malloc/free */
#define LV_MEM_CUSTOM           0

/*-----------------
 *  Internal timing
 *----------------*/

/* Use a custom sys time source (millis()) */
#define LV_TICK_CUSTOM               1
#define LV_TICK_CUSTOM_INCLUDE       "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

/*-----------------
 * Feature usage
 *----------------*/

/* Logging */
#define LV_USE_LOG               0

/* Screen transparency */
#define LV_COLOR_SCREEN_TRANSP   0

/* GPU interface (not used on ESP32) */
#define LV_USE_GPU_STM32_DMA2D   0

/*-----------------
 *  Compiler settings
 *----------------*/

/* Use C99 integer types */
#define LV_USE_64                0
#define LV_USE_DOUBLE            0

/*-----------------
 *  Font settings
 *----------------*/

/* Include at least one font—here the built-in Montserrat 14 */
#define LV_FONT_DEFAULT          &lv_font_montserrat_14
#define LV_USE_FONT_DEJAVU_12    0

/*-----------------
 *  Misc. modules
 *----------------*/

/* Enable animations (required for lv_scr_load_anim) */
#define LV_USE_ANIMATION         1

/* Enable timers (required for lv_timer_create) */
#define LV_USE_TIMER             1

/* Align objects to grid (optional) */
#define LV_USE_OBJ_REALIGN       1

/* Basic widgets you’re using */
#define LV_USE_LABEL             1
#define LV_USE_BTN               1

/* Disable unused widgets to save space */
#define LV_USE_IMG               0
#define LV_USE_SLIDER            0
#define LV_USE_LINE              0
#define LV_USE_METER             0
#define LV_USE_CHART             0
#define LV_USE_LIST              0

/*-----------------
 *  Theme
 *----------------*/

/* Default theme */
#define LV_USE_THEME_DEFAULT     1

#endif /*LV_CONF_H*/
