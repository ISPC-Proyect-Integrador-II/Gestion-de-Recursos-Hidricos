#include <Arduino.h>    
#include <Wire.h>

#define EEPROM_I2C_ADDRESS 0x50

void escribirEEPROM(int direccion, byte valor);
int leerEEPROM(int direccion);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Serial.println("Test unitario - EEPROM AT24C32");

    Serial.println(" Ingrese un número para guardar en EEPROM:");
}

void loop() {
    if (Serial.available() > 0) {
        int numero = Serial.parseInt(); // Leer número del usuario
        escribirEEPROM(0, numero); // Guardar en EEPROM

        Serial.println(" Número guardado en EEPROM. Leyendo el valor...");
        
        int valorLeido = leerEEPROM(0);
        Serial.print(" Valor leído de EEPROM: ");
        Serial.println(valorLeido);

        delay(3000);
        Serial.println("\n Ingrese otro número para guardar:");
    }
}

// Función para escribir un byte en la EEPROM
void escribirEEPROM(int direccion, byte valor) {
    Wire.beginTransmission(EEPROM_I2C_ADDRESS);
    Wire.write(direccion >> 8);
    Wire.write(direccion & 0xFF);
    Wire.write(valor);
    Wire.endTransmission();
    delay(5);
}

// Función para leer un byte de la EEPROM
int leerEEPROM(int direccion) {
    Wire.beginTransmission(EEPROM_I2C_ADDRESS);
    Wire.write(direccion >> 8);
    Wire.write(direccion & 0xFF);
    Wire.endTransmission();

    Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);
    if (Wire.available()) {
        return Wire.read();
    }
    return -1;
}