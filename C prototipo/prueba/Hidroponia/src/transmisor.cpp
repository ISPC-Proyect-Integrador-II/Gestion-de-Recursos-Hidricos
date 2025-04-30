#include "transmisor.h"

bool esNodoTransmisor = true; // Modifica este valor para activar o desactivar el modo transmisor

void inicializarTransmisor() {
    Serial.println("Modo: TRANSMISOR inicializado");
    // Aquí podrías agregar inicializaciones específicas para la transmisión (por ejemplo, configurar BLE Mesh o LoRa si fuera necesario)
}

void enviarDatos() {
    if (!esNodoTransmisor)
        return;
    
    // Se genera un mensaje JSON con algunos datos relevantes.
    // Puedes ampliar el JSON para incluir todos los parámetros que necesites.
    String datos = "{\"tempAgua\":" + String(temperaturaAgua) +
                   ",\"ph\":" + String(phValor) +
                   ",\"nivelAgua\":" + String(nivelAgua) + "}";
    
    // Publica los datos en el tópico configurado para recibir información hidropónica
    client.publish("hidroponia/datos", datos.c_str());
    
    // También podrías invocar funciones para enviar datos por BLE o LoRa, dependiendo de tu configuración.
    // enviarDatosBLE();
    // enviarDatosLoRa();
} 
