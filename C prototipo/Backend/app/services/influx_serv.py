# app/services/influx_serv.py
from app.utils.logger import get_logger
from app.models.mensaje import GatewayMessage
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
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
    logger.debug(f"Guardando en InfluxDB: {msg}")
    for node in msg.nodes:
        point = (
            Point("sensor_data")
            # tags
            .tag("gateway",    msg.gatewayId)
            .tag("device",     node.deviceId)
            .tag("transmitter", str(node.transmitter))
            .tag("reciber",     str(node.reciber))
            # timestamp con precisión de nanosegundos
            .time(node.timestamp, WritePrecision.NS)
        )

        # fields de sensores (excluyendo 'hora' si existe)
        sensor_vals = node.sensors.dict(exclude_none=True)
        sensor_vals.pop("hora", None)
        for k, v in sensor_vals.items():
            point.field(k, v)

        # fields de controles
        for k, v in node.controls.dict().items():
            point.field(k, v)

        # escribe en el bucket y org correctos
        _write_api.write(
            bucket=settings.influx_bucket,
            org=settings.influx_org,
            record=point
        )

    logger.info("Datos escritos en InfluxDB.")
