from typing import Optional
from app.utils.logger import get_logger
from app.models.mensaje import GatewayMessage
import sqlalchemy as sa
from sqlalchemy import Table, MetaData, Column, String, DateTime, Boolean, func
from app.config import settings

logger = get_logger("mysql_service")

#========================================
# Configuración SQLAlchemy
#========================================
_engine = sa.create_engine(settings.mysql_url)
_metadata = MetaData()
_device_table = Table(
    "device_info",
    _metadata,
    Column("device_id", String(64), primary_key=True),
    Column("name", String(128), nullable=False),
    Column("created_at", DateTime, nullable=False, server_default=func.current_timestamp()),
    Column("transmitter", Boolean, nullable=False, default=False),
    Column("receiver", Boolean, nullable=False, default=False),
    Column("gateway", Boolean, nullable=False, default=False),
)
_metadata.create_all(_engine)

#========================================
# Funcion que registra info en mysql
#========================================
def save_device_info(msg: GatewayMessage) -> None:
    logger.debug(f"Guardando información de dispositivos en MySQL: {msg}")
    with _engine.connect() as conn:
        for node in msg.nodes:
            gateway_flag = True if node.deviceId == msg.gatewayId else False
            ins = sa.insert(_device_table).values(
                device_id=node.deviceId,
                name=node.deviceId,
                transmitter=node.transmitter or False,
                receiver=node.reciber or False,
                gateway=gateway_flag
            ).prefix_with("IGNORE")
            conn.execute(ins)
        conn.commit()
    logger.info("Información de dispositivos escrita en MySQL.")