#ifndef MENU_H
#define MENU_H

#include "pantalla.h"
#include "comunicacion/comunicacion.h"
#include "sensores/sensores.h"

// Funciones para mostrar y procesar la navegación del menú general
void mostrarMenuPrincipal();
void procesarEntradaMenu();
void manejarSeleccionMenu();
void mostrarMenuRecetas();
void mostrarMenuWiFi();
void mostrarMensaje(String mensaje);


#endif 

