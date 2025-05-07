from datetime import datetime
from pydantic import BaseModel

#========================================
# FORMATO DE LOS MENSAJES
#========================================
class Message(BaseModel):
    topic: str
    payload: dict
    timestamp: datetime


# A definir si vamos a trozear el mensaje o no, y que parametros tomamos de cada json.    