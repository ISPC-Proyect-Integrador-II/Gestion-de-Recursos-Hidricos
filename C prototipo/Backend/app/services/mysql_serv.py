from typing import List
from app.utils.logger import get_logger

logger = get_logger("mysql_service")


#========================================
# FUNCION REGISTRAR MENSAJE EN MYSQL
#========================================
def save_message(data: dict) -> None:
    """
    Guarda un mensaje en la base de datos MySQL.
    A definir, una vez diseñada la bd
    """
    logger.debug(f"Guardando en MySQL: {data}")
    # TODO: Conectar a MySQL usando settings.mysql_url y escribir el registro
    pass

#========================================
# FUNCION MOSTRAR MENSAJES EN MYSQL
#========================================
def get_recent_messages() -> List[dict]:
    """
    Retorna una lista de mensajes desde MySQL.
    Implementa la consulta adecuada. A definir, una vez diseñada la bd
    """
    logger.debug("Obteniendo mensajes recientes de MySQL")
    # TODO: Ejecutar SELECT y mapear a dict/lista
    return []