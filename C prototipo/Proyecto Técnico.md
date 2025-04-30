# 🌱 Proyecto Técnico: Sistema Hidropónico Automatizado con ESP32



## 🧠 Microcontrolador

- **Placa principal:** NodeMCU ESP32 WROOM (38 pines)
- **Tensión de operación:** 3.3V
- **Tensión de alimentación:** 5V vía USB
- **Comunicación:** Wi-Fi, Bluetooth, UART, SPI, I2C

---

## 📌 Distribución de Pines del ESP32

| Sensor / Módulo                        | Tipo de conexión        | Pines ESP32          |
|----------------------------------------|--------------------------|-----------------------|
| Bomba Sumergible (control vía relé)    | Digital                  | GPIO 26              |
| Buzzer Pasivo                          | PWM / Digital            | GPIO 27              |
| Caudalímetro (0.3–6 L/min)             | Pulso digital            | GPIO 18              |
| Sensor de Corriente INA219             | I2C                      | SDA = GPIO 21, SCL = GPIO 22 |
| RTC DS1307 + EEPROM 24C32              | I2C                      | SDA = GPIO 21, SCL = GPIO 22 |
| Sensor de Temperatura DS18B20          | 1-Wire                   | GPIO 33              |
| Sensor de Gas MQ7                      | Analógico                | GPIO 36 (VP)         |
| Display TFT ST7735 (SPI)               | SPI                      | CS = GPIO 5, DC = GPIO 2, RESET = GPIO 4, MOSI = GPIO 23, SCK = GPIO 18 |
| Sensor DHT11                           | Digital                  | GPIO 25              |
| Sensor LDR                             | Analógico                | GPIO 39 (VN)         |
| Sensor Ultrasonido HC-SR04             | Digital                  | TRIG = GPIO 12, ECHO = GPIO 14 |
| Módulo GSM SIM800L                     | UART                     | RX = GPIO 16, TX = GPIO 17 |
| Módulo LoRa SX1278 (SPI)               | SPI                      | NSS = GPIO 15, MOSI = GPIO 23, MISO = GPIO 19, SCK = GPIO 18, RST = GPIO 13, DIO0 = GPIO 34 |
| Relay 1 Canal                          | Digital                  | GPIO 26              |

---

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
- **Señal (pulso)** → GPIO 18

---

### 4. **INA219**
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

### 6. **DS18B20**
- **VCC** → 3.3V
- **GND** → GND
- **DATA** → GPIO 33
- **Resistencia** (pull-up 4.7kΩ entre DATA y VCC)

---

### 7. **MQ7 (CO)**
- **VCC** → 5V
- **GND** → GND
- **AOUT** → GPIO 36

---

### 8. **Display TFT ST7735**
- **VCC** → 3.3V
- **GND** → GND
- **CS** → GPIO 5
- **RESET** → GPIO 4
- **DC** → GPIO 2
- **MOSI** → GPIO 23
- **SCK** → GPIO 18

---

### 9. **DHT11**
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
- **ECHO** → GPIO 14 *(usar 3.3V)*

---

### 12. **SIM800L**
- **VCC** → Fuente externa 4V
- **GND** → GND común
- **TX (SIM)** → GPIO 16 (RX ESP32)
- **RX (SIM)** → GPIO 17 (TX ESP32) *(usar 3.3V)*

---

### 13. **LoRa SX1278**
- **VCC** → 3.3V
- **GND** → GND
- **MOSI** → GPIO 23
- **MISO** → GPIO 19
- **SCK** → GPIO 18
- **NSS (CS)** → GPIO 15
- **RESET** → GPIO 13
- **DIO0** → GPIO 34

---

## ⚙️ Funcionalidades del Sistema

- ✅ Control automático de riego con bomba sumergible
- ✅ Medición de caudal con sensor de flujo
- ✅ Monitoreo de temperatura ambiental y del agua
- ✅ Medición de concentración de CO2 
- ✅ Visualización en pantalla TFT 
- ✅ Envío de alertas por SMS vía GSM (SIM800L)
- ✅ Sincronización horaria con RTC DS1307
- ✅ Nivel de tanque por ultrasonido
- ✅ Comunicación remota por radiofrecuencia (LoRa)
- ✅ Detección de condiciones de luz (cultivo LED o solar)

