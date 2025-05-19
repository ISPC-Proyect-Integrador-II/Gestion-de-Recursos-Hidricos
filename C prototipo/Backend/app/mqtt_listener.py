import json
import threading
import paho.mqtt.client as mqtt
from app.config import settings
from app.utils.logger import get_logger
from app.services import mysql_serv, influx_serv
from app.models.mensaje import GatewayMessage

logger = get_logger("mqtt_listener")

#========================================
# Configuración de la aplicación (CLASE)
#========================================

class MQTTListener:
    """
    Se encarga de:
      - Conectar al broker MQTT
      - Suscribirse a los topics indicados en settings.mqtt_topics
      - Procesar cada mensaje y derivarlo al servicio adecuado
    """
    #------------------------------------------
    # CREACION DEL CLIENTE MQTT
    #------------------------------------------
    def __init__(self):
        # Creamos el cliente MQTT
        self.client = mqtt.Client()
        # Si definimos que el broker requiere usuario/clave, descomentar:
        if settings.mqtt_user and settings.mqtt_password:
            self.client.username_pw_set(settings.mqtt_user, settings.mqtt_password)
        self.client.on_connect = self._on_connect
        self.client.on_message = self._on_message

    #------------------------------------------
    # ARRAQUE DE CONEXION CON EL BROKER
    #------------------------------------------
    def start(self):
        logger.info(f"Conectando a broker MQTT en {settings.mqtt_broker_url}...")
        # Asumimos URL con puerto, ej: "mqtt://host:1883" -> extraemos host/puerto
        url = settings.mqtt_broker_url.replace("mqtt://", "")
        host, port = url.split(":")
        self.client.connect(host, int(port))
        # Levantamos el loop en background para no bloquear la app HTTP
        threading.Thread(target=self.client.loop_forever, daemon=True).start()
        logger.info("MQTT listener arrancado en background.")

    #------------------------------------------
    # ACCIONES AL CONECTAR (SUBSCRIPCION A TOPICS)
    #------------------------------------------
    def _on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            logger.info("Conexión MQTT exitosa.")
            # Nos suscribimos a cada topic
            for topic in settings.mqtt_topics:
                client.subscribe(topic)
                logger.info(f"Suscrito al topic '{topic}'.")
        else:
            logger.error(f"Fallo al conectar MQTT, código de error: {rc}")

    #------------------------------------------
    # ACCIONES AL RECIBIR MENSAJE
    #------------------------------------------
    def _on_message(self, client, userdata, msg):
        # Decodifica y parsea el JSON
        try:
            payload = msg.payload.decode("utf-8")
            data = json.loads(payload)
            logger.debug(f"Mensaje recibido en '{msg.topic}': {data}")
        except Exception as e:
            logger.error(f"Error parseando JSON en topic '{msg.topic}': {e}")
            return

        if any(msg.topic.startswith(t) for t in settings.mqtt_topics):
            try:
                # Validación del modelo de datos
                mensaje = GatewayMessage.parse_obj(data)
                # Guardado en ambos servicios
                mysql_serv.save_device_info(mensaje)
                influx_serv.save_message(mensaje)
                logger.info("Guardado en MySQL e InfluxDB.")
            except Exception as ex:
                logger.error(f"Error guardando en servicios de BD: {ex}")
        else:
            logger.warning(f"Topic '{msg.topic}' no mapeado a ningún servicio.")


#------------------------------------------
# INTREGRACION EN MAIN.PY
#------------------------------------------
listener = MQTTListener()
