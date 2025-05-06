#ifndef CONTROL_H
#define CONTROL_H
extern void activarIluminacion(bool estado);

#include "sensores/sensores.h"
#include "comunicacion/comunicacion.h"

// Definiciones de seguridad y límites
#define MIN_TEMP_AGUA 18.0
#define MAX_TEMP_AGUA 28.0
#define MIN_TEMP_AIRE 15.0
#define MAX_TEMP_AIRE 35.0
#define MIN_HUMEDAD 40.0
#define MAX_HUMEDAD 80.0
#define MIN_NIVEL_AGUA 5.0  // cm desde el fondo
#define MIN_FLUJO_AGUA 0.5   // L/min cuando la bomba está encendida
#define MAX_GAS 800          // Nivel arbitrario, ajustar según calibración
#define MIN_PH 5.5
#define MAX_PH 7.5
#define MIN_LUZ 300
#define MAX_LUZ 800


// Estado del sistema
extern bool bomba_activa;
extern bool luces_activas;
extern bool alarma_activa;
extern bool modo_automatico;

// Inicialización del control
void inicializarControl();

// Verificación de alarmas
void verificarAlarmas();

// Activación de bomba
void activarBomba(bool estado);

// Activación de iluminación
void activarIluminacion(bool estado);

// Control automático
void controlAutomatico();

// Notificación de estado general
void notificarEstado();

#endif 
