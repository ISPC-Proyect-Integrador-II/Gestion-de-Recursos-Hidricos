#pragma once
#include <Arduino.h>

/**
 * @brief Control de bomba por umbral o temporizador, con comando remoto.
 */
class bomba {
public:
    bomba(uint8_t PIN_BOMBA, int umbral);

    /** Configura el pin y apaga la bomba */
    void iniciar();

    /**
     * @brief Llamar en cada loop().
     * - Si hay temporizador activo, espera a que expire y apaga la bomba.
     * - Si no, simula (o luego leerá) el sensor y enciende/apaga según el umbral.
     */
    void actualizar();

    /**
     * @brief Procesa comando remoto vía MQTT:
     *        "ON"  → arranca la bomba indefinidamente
     *        "OFF" → apaga la bomba
     *        "<n>" → arranca la bomba por n segundos
     */
    void comando(const String& msg);

    /** Ajusta el umbral en tiempo de ejecución */
    void establecer_umbral(int umbral);

    /** Getters para consultar estado sin acceder a privados */
    bool isOn() const;
    bool isTimerActive() const;

private:
    void encender_bomba();
    void frenar_bomba();

    uint8_t      PIN_BOMBA_;
    int          umbral_;
    bool         estado_bomba_;
    bool         estado_timer_;
    unsigned long timerInicio_;
    unsigned long timerDuracion_;
};
