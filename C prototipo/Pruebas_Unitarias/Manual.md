## Manual de uso de directorio de pruebas unificada.

### Cual es la idea?
Para que no tengamos que crear un proyecto de platofmio nuevo por cada sensor, que ademas deberia incluir las funciones de conexion, probe crear un direcotiro, modulado con el que podamos crear el codigo de de cada sensor, pero cuando lo compilemos solo llamemos al/los que querramos testear. Sin tener que modificar las conexiones (excepto las credenciales/broker) 

### Conexiones
De momento, solo MQTT y Wifi. Busque crearlas asincronicas, de forma que corra lo que sea, sin problema en ese apartado.
Idealmente, se pueden agregar otras funciones de conexion y deberiamos tener que modificar demasiado el codigo

## Estructura del Directorio:

- 📦Pruebas_Unitarias
-  ┣ 📂.pio
-  ┣ 📂.vscode
-  ┣ 📂include              // Aca van las librerias creadas de mqtt, wifi, o las proximas conexiones o elementos fijos
-  ┃ ┣ 📜mqtt_async.h
-  ┃ ┣ 📜README
-  ┃ ┗ 📜wifi_async.h
-  ┣ 📂lib
-  ┃ ┗ 📜README
-  ┣ 📂src                  // Aca van las funciones correspondientes a los .h fijos tambien
-  ┣ ┗ 📂sensores             // Aca creamos la carpeta de los sensores que vayamos a testear. 
-  ┃ ┃┗ 📂sr04
-  ┃ ┃  ┣ 📜sr04.cpp
-  ┃ ┃  ┗ 📜sr04.h
-  ┃ ┣ 📜main.cpp
-  ┃ ┣ 📜mqtt_async.cpp
-  ┃ ┗ 📜wifi_async.cpp
-  ┣ 📂test
-  ┃ ┗ 📜README
-  ┣ 📜.gitignore
-  ┣ 📜Manual.md_
-  ┗ 📜platformio.ini       // Vamos a tener que modificar el src_filter cada vez que busquemos compilarlo para probar.

## Como usarlo
Como comente, la intencion es poder agregar uno a uno los sensores, agregar su correspondiente codigo y documentos, pero utilizar solo los que necesitemos em la prueba.

- Crear la carpeta y respectivos [sensor].h y [sensor].cpp
- Modificar el platforio.ini [src_filter]
    
    build_src_filter =
    +<*.cpp>                ; tus .cpp en la raíz de src/
    +<sensores/sr04/*.cpp>    ; todo lo que haya bajo src/sensores/ cambiarndo la carpeta "sr04" 

    // Creamos la bandera con el formato [-D]+SENSOR_[SENSOR] Queda algo como en el ejemplo
    build_flags = -DSENSOR_SR04

- Agregamos lo siguiente en el main:
    
    // Agregamos un **ifdef** + [flag]. Ahi incluimos el archivo.h indicando la direccion del que vayamos a usar y los pines.
    #ifdef SENSOR_SR04
    #include "sensores/sr04/sr04.h"
    #define TRIG_PIN 5
    #define ECHO_PIN 18
    #endif

    // Agregamos un **ifdef** + [flag] en el setup con el setup correspondiente para el sensor
    // Agregamos un **ifdef** + [flag] en el loop con las funciones correspondiente para el sensor

**NO HACE FALTA BORRAR FUNCIONES DE OTROS SENOSRES, SINO ESTA EL FLAG DEL PRINCIPIO, NO DEBERIAN INTERRUMPIR LA EJECUCION**