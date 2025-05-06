#ifndef GSM_ASYNC_H
#define GSM_ASYNC_H

#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

/// Inicia y gestiona GSM/GPRS 
void inicializarGsm(const char* apn, const char* user, const char* pass);
/// Loop en el que chequea y mantiene la conexion
void gsmLoop();
/// Indica el estado de la conexion
bool estadoGsm();
/// Proporciona el cliente TCP para MQTT
TinyGsmClient& gsmGetClient();

#endif