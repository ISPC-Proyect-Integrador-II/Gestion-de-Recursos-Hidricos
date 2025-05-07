from typing import List
from app.utils.logger import get_logger

logger = get_logger("influx_service")

#========================================
# FUNCION REGISTRAR MENSAJE EN INFLUX
#========================================
def save_message(data: dict) -> None:
    """
    Guarda un mensaje en InfluxDB.
    Implementa aquí la lógica de conexión y escritura.
    """
    logger.debug(f"Guardando en InfluxDB: {data}")
    # TODO: Conectar a InfluxDB usando settings.influx_url y escribir el registro
    pass

#========================================
# FUNCION MOSTRAR MENSAJES EN INFLUX
#========================================
def get_recent_messages() -> List[dict]:
    """
    Retorna una lista de mensajes desde InfluxDB.
    Implementa la consulta adecuada.
    """
    logger.debug("Obteniendo mensajes recientes de InfluxDB")
    # TODO: Ejecutar consulta y mapear a dict/lista
    return []
