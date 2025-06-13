from datetime import datetime
from app.utils.logger import get_logger
from app.models.mensaje import GatewayMessage
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from influxdb_client.client.exceptions import InfluxDBError
from app.config import settings

logger = get_logger("influx_service")

# Cliente configurado con URL, token y org desde .env
_influx_client = InfluxDBClient(
    url=settings.influx_url,
    token=settings.influx_token,
    org=settings.influx_org
)
_write_api = _influx_client.write_api(write_options=SYNCHRONOUS)

def save_message(msg: GatewayMessage) -> None:
    """
    Registra la información de un GatewayMessage en InfluxDB usando timestamp de ingestión.
    """
    measurement = "sensor_data"
    for node in msg.nodes:
        # No especificar .time() para usar timestamp de ingestión (now())
        point = Point(measurement) \
            .tag("gatewayId", msg.gatewayId) \
            .tag("deviceId", node.deviceId)

        # fields de sensores
        for k, v in node.sensors.dict().items():
            if isinstance(v, datetime):
                logger.debug(f"Skipping sensor field '{k}' of type datetime")
                continue
            point.field(k, v)

        # fields de controles
        for k, v in node.controls.dict().items():
            point.field(k, v)

        # Debug: mostrar línea en protocolo InfluxDB
        line = point.to_line_protocol()
        logger.debug(f"InfluxDB line protocol payload: {line}")

        try:
            # Escribir en InfluxDB con timestamp actual
            _write_api.write(
                bucket=settings.influx_bucket,
                org=settings.influx_org,
                record=point
            )
            logger.info(f"InfluxDB write successful for point: {line}")
        except InfluxDBError as e:
            logger.error(f"Error escribiendo en InfluxDB: {e}. Payload: {line}")
            continue

    logger.info("Proceso de escritura en InfluxDB completado.")