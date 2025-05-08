#include "transmisor.h"

//=======================================================
// Marca este ESP32 como transmisor 
//=======================================================

bool esNodoTransmisor = true; // cambia a false para desactivar envío :contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3}

//=======================================================
// INICIALIZACIÓN DEL TRANSMISOR
//=======================================================

void inicializarTransmisor() {
    Serial.println("Modo: TRANSMISOR inicializado");
    // Si usaras LoRa o BLE Mesh:
    // if (loRaActivo) conectarLoRa();
    // if (bleActivo)  conectarBLEMesh();
}
