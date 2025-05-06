#include "pantalla/pantalla.h"
#include "sensores/sensores.h"
#include "errores/errores.h"

// Objeto para la pantalla
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Variables de tiempo
unsigned long ultimoParpadeo = 0;
unsigned long ultimoAlerta = 0;
bool ojosAbiertos = true;

// Estado de la pantalla
bool modoStandby = false;
bool alertaAgua = false;

// Inicialización de la pantalla
bool inicializarPantalla() {
    tft.initR(INITR_BLACKTAB);
    CHECK(tft.width() > 0, ErrorCode::DISPLAY_INIT_FAIL); // Verifica que la pantalla se haya inicializado correctamente
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);

    pinMode(BOTON_IZQUIERDA, INPUT_PULLUP);
    pinMode(BOTON_DERECHA, INPUT_PULLUP);
    pinMode(BOTON_CONFIRMAR, INPUT_PULLUP);

    return true;
}

// Actualización de pantalla
void actualizarPantalla() {
    if (modoStandby) {
        mostrarStandby();
    } else if (alertaAgua) {
        mostrarAlertaAgua();
    } else {
        mostrarDatos();
    }
}

// Animación de ojos en standby
void mostrarStandby() {
    unsigned long tiempoActual = millis();
    if (tiempoActual - ultimoParpadeo > STANDBY_INTERVAL) {
        ojosAbiertos = !ojosAbiertos;
        ultimoParpadeo = tiempoActual;
    }

    tft.fillScreen(ST7735_BLACK);
    if (ojosAbiertos) {
        tft.fillCircle(40, 60, 15, ST7735_WHITE);
        tft.fillCircle(80, 60, 15, ST7735_WHITE);
    } else {
        tft.fillRect(30, 55, 60, 10, ST7735_WHITE);
    }
}

// Animación de alerta de agua
void mostrarAlertaAgua() {
    unsigned long tiempoActual = millis();
    if (tiempoActual - ultimoAlerta > ALERTA_INTERVAL) {
        alertaAgua = !alertaAgua;
        ultimoAlerta = tiempoActual;
    }

    tft.fillScreen(alertaAgua ? ST7735_RED : ST7735_BLACK);
    tft.setCursor(20, 50);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(2);
    tft.println("¡AGUA BAJA!");
}

// Manejo de botones para navegación
void manejarBotones() {
    if (digitalRead(BOTON_IZQUIERDA) == LOW) {
        modoStandby = !modoStandby;
        delay(200);
    }
    
    if (digitalRead(BOTON_DERECHA) == LOW) {
        alertaAgua = !alertaAgua;
        delay(200);
    }
} 
void mostrarDatos() {
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println("Temp Agua: " + String(temperaturaAgua) + "°C");
    tft.println("Nivel Agua: " + String(nivelAgua) + "cm");
    tft.println("pH: " + String(phValor));

    // Puedes agregar más información de sensores aquí
}
void mostrarMensaje(String mensaje) {
    Serial.println(mensaje);
}
