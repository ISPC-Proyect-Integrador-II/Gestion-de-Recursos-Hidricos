from pydantic import BaseSettings, AnyHttpUrl
from typing import List

#========================================
# Configuración de la aplicación
#========================================
class Settings(BaseSettings):

    #------------------------------------------
    # MQTT
    #------------------------------------------
    mqtt_broker_url: str
    mqtt_topics: List[str]

    #------------------------------------------
    # BASES DE DATOS
    #------------------------------------------
    mysql_url: str
    influx_url: str

    #------------------------------------------
    # APLICACION PYTHON
    #------------------------------------------
    api_host: str = "0.0.0.0"
    api_port: int = 8000

    #------------------------------------------
    # ENVIRONMENT
    #------------------------------------------
    class Config:
        env_file = ".env"
        env_file_encoding = "utf-8"

settings = Settings()
