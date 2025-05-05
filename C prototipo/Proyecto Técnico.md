# 🌱 Sistema Hidropónico Automatizado con ESP32

## 🧠 Microcontrolador
- **Placa principal:** NodeMCU ESP32 WROOM (38 pines)
- **Tensión de operación:** 3.3V
- **Tensión de alimentación:** 5V vía USB
- **Comunicación:** Wi-Fi, Bluetooth, UART, SPI, I2C

## 📌 Mapeo Completo de Pines ESP32 (NodeMCU ESP32 WROOM)

| Pin | GPIO | Nombre | Estado | Uso en Proyecto |
|-----|------|--------|--------|----------------|
| 1 | - | 3V3 | Ocupado | Alimentación para sensores de 3.3V |
| 2 | GPIO23 | IO23 | Ocupado | LoRa MOSI |
| 3 | GPIO22 | IO22/SCL | Ocupado | I2C SCL (INA219, RTC DS1307) |
| 4 | GPIO1 | TX | Libre | - |
| 5 | GPIO3 | RX | Libre | - |
| 6 | GPIO21 | IO21/SDA | Ocupado | I2C SDA (INA219, RTC DS1307) |
| 7 | GND | GND | Ocupado | Tierra común |
| 8 | GPIO19 | IO19 | Ocupado | LoRa MISO |
| 9 | GPIO18 | IO18 | Ocupado | LoRa SCK |
| 10 | GPIO5 | IO5 | Ocupado | LoRa NSS |
| 11 | GPIO17 | IO17 | Ocupado | SIM800L TX |
| 12 | GPIO16 | IO16 | Ocupado | SIM800L RX |
| 13 | GPIO4 | IO4 | Libre | - |
| 14 | GPIO0 | IO0 | Ocupado | Display TFT RESET |
| 15 | GPIO2 | IO2 | Libre | - |
| 16 | GPIO15 | IO15 | Ocupado | LoRa RST |
| 17 | GPIO13 | IO13 | Ocupado | Display TFT CS |
| 18 | GPIO12 | IO12 | Ocupado | Display TFT DC, HC-SR04 TRIG |
| 19 | GPIO14 | IO14 | Ocupado | Display TFT MOSI |
| 20 | GPIO27 | IO27 | Ocupado | Display TFT SCK, Buzzer Pasivo |
| 21 | GPIO26 | IO26 | Ocupado | Bomba Sumergible (Relay 1 Canal) |
| 22 | GPIO25 | IO25 | Ocupado | DHT11/DHT22 DATA |
| 23 | GPIO33 | IO33 | Ocupado | DS18B20 DATA |
| 24 | GPIO32 | IO32 | Ocupado | HC-SR04 ECHO |
| 25 | GPIO35 | IO35 | Ocupado | Relay 2 Canal |
| 26 | GPIO34 | IO34 | Ocupado | LoRa DIO0 |
| 27 | GPIO39 | IO39/VN | Ocupado | Sensor LDR, Medidor de pH |
| 28 | GPIO36 | IO36/VP | Ocupado | Sensor MQ-7 |
| 29 | EN | EN | Sistema | - |
| 30 | - | 5V | Ocupado | Alimentación para sensores de 5V |

## 📌 Distribución de Pines para Sensores

| Sensor / Módulo | Tipo de conexión | Pines ESP32 |
|-----------------|------------------|-------------|
| **Bomba Sumergible (Relay)** | Digital | GPIO 26 |
| **Buzzer Pasivo** | PWM / Digital | GPIO 27 |
| **Caudalímetro** | Pulso digital | GPIO 32 |
| **Sensor de Corriente INA219** | I2C | SDA = GPIO 21, SCL = GPIO 22 |
| **RTC DS1307 + EEPROM AT24C32** | I2C | SDA = GPIO 21, SCL = GPIO 22 |
| **Sensor de Temperatura DS18B20** | 1-Wire | GPIO 33 |
| **Sensor de Gas MQ7** | Analógico | GPIO 36 (VP) |
| **Display TFT ST7735 (SPI)** | SPI | CS = GPIO 13, DC = GPIO 12, RESET = GPIO 0, MOSI = GPIO 14, SCK = GPIO 27 |
| **Sensor DHT11/DHT22** | Digital | GPIO 25 |
| **Sensor de Luz LDR** | Analógico | GPIO 39 (VN) |
| **Sensor Ultrasonido HC-SR04** | Digital | TRIG = GPIO 12, ECHO = GPIO 32 |
| **Módulo GSM SIM800L** | UART | RX = GPIO 16, TX = GPIO 17 |
| **Módulo LoRa SX1278 (SPI)** | SPI | NSS = GPIO 5, MOSI = GPIO 23, MISO = GPIO 19, SCK = GPIO 18, RST = GPIO 15, DIO0 = GPIO 34 |
| **Relay 1 Canal** | Digital | GPIO 26 |
| **Relay 2 Canal** | Digital | GPIO 35 |
| **Medidor de pH** | ADC | GPIO 39 (ADC1_CH3) |

## 🔌 Conexiones de Módulos y Sensores

### 1. **Bomba Sumergible + Relay**
- **Relay VCC** → 5V
- **Relay GND** → GND
- **Relay IN** → GPIO 26
- **COM (Relay)** → + de la bomba
- **NO (Relay)** → + de fuente de 5V externa
- **Bomba GND** → GND fuente

---

### 2. **Buzzer Pasivo**
- **VCC** → 5V
- **GND** → GND
- **Señal** → GPIO 27

---

### 3. **Caudalímetro**
- **VCC** → 5V
- **GND** → GND
- **Señal (pulso)** → GPIO 32

---

### 4. **Sensor de Corriente INA219**
- **VCC** → 3.3V
- **GND** → GND
- **SDA** → GPIO 21
- **SCL** → GPIO 22

---

### 5. **RTC DS1307 + EEPROM**
- **VCC** → 3.3V o 5V
- **GND** → GND
- **SDA** → GPIO 21
- **SCL** → GPIO 22

---

### 6. **Sensor DS18B20**
- **VCC** → 3.3V
- **GND** → GND
- **DATA** → GPIO 33
- **Resistencia** (pull-up 4.7kΩ entre DATA y VCC)

---

### 7. **Sensor MQ-7 (CO)**
- **VCC** → 5V
- **GND** → GND
- **AOUT** → GPIO 36

---

### 8. **Display TFT ST7735**
- **VCC** → 3.3V
- **GND** → GND
- **CS** → GPIO 13
- **RESET** → GPIO 0
- **DC** → GPIO 12
- **MOSI** → GPIO 14
- **SCK** → GPIO 27

---

### 9. **Sensor DHT11/DHT22**
- **VCC** → 3.3V
- **GND** → GND
- **DATA** → GPIO 25

---

### 10. **Sensor de Luz (LDR)**
- Un extremo del **LDR** → 3.3V
- Otro extremo → resistencia de 10kΩ a GND y punto medio al GPIO 39

---

### 11. **HC-SR04**
- **VCC** → 5V
- **GND** → GND
- **TRIG** → GPIO 12
- **ECHO** → GPIO 32 *(usar divisor de voltaje para 3.3V)*

---

### 12. **SIM800L (Módulo GSM)**
- **VCC** → Fuente externa 4V
- **GND** → GND común
- **TX (SIM800L)** → GPIO 16 (RX ESP32)
- **RX (SIM800L)** → GPIO 17 (TX ESP32) *(usar 3.3V)*

---

### 13. **LoRa SX1278**
- **VCC** → 3.3V
- **GND** → GND
- **MOSI** → GPIO 23
- **MISO** → GPIO 19
- **SCK** → GPIO 18
- **NSS (CS)** → GPIO 5
- **RESET** → GPIO 15
- **DIO0** → GPIO 34

---

### 14. **Medidor de pH**
- **VCC** → 5V
- **GND** → GND
- **Salida Analógica** → GPIO 39 *(ADC1_CH3)*

---

## 📋 Lista de Materiales

| Componente | Cantidad | Descripción |
|------------|----------|-------------|
| NodeMCU ESP32 WROOM | 1 | Microcontrolador con WiFi y Bluetooth (38 pines) |
| Relay 1 Canal | 1 | Para control de bomba sumergible |
| Relay 2 Canal | 1 | Para control de actuadores adicionales |
| Buzzer Pasivo | 1 | Para alertas sonoras |
| Caudalímetro | 1 | Sensor de flujo de agua (0.3-6 L/min) |
| INA219 | 1 | Sensor de corriente de alta precisión |
| DS1307 + AT24C32 | 1 | Reloj de tiempo real con EEPROM |
| DS18B20 | 1 | Sensor de temperatura resistente al agua |
| MQ-7 | 1 | Sensor de monóxido de carbono |
| Display TFT ST7735 | 1 | Pantalla a color SPI 1.8" |
| DHT11/DHT22 | 1 | Sensor de temperatura y humedad |
| LDR | 1 | Fotorresistor para medir intensidad de luz |
| HC-SR04 | 1 | Sensor ultrasónico de distancia |
| SIM800L | 1 | Módulo GSM para comunicación celular |
| LoRa SX1278 | 1 | Módulo de comunicación de largo alcance |
| Sensor de pH | 1 | Medidor del pH del agua |
| Bomba Sumergible | 1 | Bomba de agua para circulación |
| Resistencias | Varias | 10kΩ (para LDR), 4.7kΩ (para DS18B20), divisor de voltaje |
| Cables Dupont | Varios | Para interconexión de componentes |
| Protoboard | 1 | Para montaje de circuito |
| Fuente de alimentación | 1 | 5V/2A para ESP32 y 4V para SIM800L |

## 🔄 Tipos de Conexiones

### I2C (Inter-Integrated Circuit) – Bus compartido
Todos los módulos que usan I2C están en el mismo bus para reducir consumo y mejorar estabilidad:
| Sensor / Módulo | Dirección I2C | Pines ESP32 |
|-----------------|---------------|-------------|
| Sensor de Corriente INA219 | 0x40 | SDA = GPIO 21, SCL = GPIO 22 |
| RTC DS1307 + EEPROM AT24C32 | 0x68 / 0x57 | SDA = GPIO 21, SCL = GPIO 22 |

### SPI (Serial Peripheral Interface) – Bus compartido
Para mejorar eficiencia, algunos dispositivos comparten el bus SPI con pines CS independientes:
| Sensor / Módulo | Pines ESP32 |
|-----------------|-------------|
| Display TFT ST7735 | MOSI = GPIO 14, SCK = GPIO 27, CS = GPIO 13 |
| Módulo LoRa SX1278 | MOSI = GPIO 23, MISO = GPIO 19, SCK = GPIO 18, CS = GPIO 5 |

### Entradas Analógicas
El ESP32 cuenta con ADC (convertidor analógico a digital) en varios pines:
| Sensor / Módulo | Canal ADC | Pines ESP32 |
|-----------------|-----------|-------------|
| Sensor LDR | ADC1_CH3 | GPIO 39 (VN) |
| Sensor MQ-7 | ADC1_CH0 | GPIO 36 (VP) |
| Medidor de pH | ADC1_CH3 | GPIO 39 (VN) |

## ⚙️ Funcionalidades del Sistema
✅ Control automático de riego con bomba sumergible  
✅ Medición de caudal con sensor de flujo  
✅ Monitoreo de temperatura ambiental y del agua  
✅ Medición de concentración de CO2  
✅ Visualización en pantalla TFT  
✅ Envío de alertas por SMS vía GSM (SIM800L)  
✅ Sincronización horaria con RTC DS1307  
✅ Nivel de tanque por ultrasonido  
✅ Comunicación remota por radiofrecuencia (LoRa)  
✅ Detección de condiciones de luz (cultivo LED o solar)  
✅ Medición de pH del agua