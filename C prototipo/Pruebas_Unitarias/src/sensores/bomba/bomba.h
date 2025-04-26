#pragma once
#include <Arduino.h>

/**
 * @brief Control de bomba de agua con activación por umbral o temporizador.
 */
class bomba {
public:
    bomba(uint8_t PIN_BOMBA, int umbral);

    /** Configura el pin y deja la bomba apagada */
    void iniciar();

    /**
     * @brief Debe llamarse en cada loop().
     *        - Si hay timer activo, espera a que expire y para la bomba.
     *        - Si no, lee sensor (ahora simulado) y enciende/apaga según umbral.
     */
    void actualizar();

    /**
     * @brief Procesa comandos remotos:
     *        "ON"  → enciende bomba
     *        "OFF" → apaga bomba
     *        "<n>" → enciende por n segundos
     */
    void comando(const String& msg);

    /** Cambia el umbral de manera dinámica */
    void establecer_umbral(int umbral);

    /** Getters para estado externo */
    bool isOn() const;
    bool isTimerActive() const;

private:
    void encender_bomba();
    void frenar_bomba();

    uint8_t     PIN_BOMBA_;
    int         umbral_;
    bool        estado_bomba_;
    bool        estado_timer_;
    unsigned long timerInicio_;
    unsigned long timerDuracion_;
};
