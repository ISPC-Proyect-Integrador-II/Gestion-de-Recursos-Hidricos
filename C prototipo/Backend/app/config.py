from typing import List, Optional
from pydantic import Field
from pydantic_settings import BaseSettings, SettingsConfigDict

class Settings(BaseSettings):
    # MQTT
    mqtt_broker_url: str           = Field(..., env="MQTT_BROKER_URL")
    mqtt_user: Optional[str]       = Field(None,   env="MQTT_USER")
    mqtt_password: Optional[str]   = Field(None,   env="MQTT_PASSWORD")
    mqtt_topics: List[str]         = Field(...,   env="MQTT_TOPICS")

    # MySQL
    mysql_url: str                 = Field(...,   env="MYSQL_URL")

    # InfluxDB v2
    influx_url: str                = Field("http://influxdb:8086",       env="INFLUX_URL")
    influx_token: str              = Field("supersecrettoken",          env="INFLUX_INIT_ADMIN_TOKEN")
    influx_org: str                = Field("hidroponia_org",            env="INFLUX_INIT_ORG")
    influx_bucket: str             = Field("sensor_data",                env="INFLUX_INIT_BUCKET")

    # App HTTP
    api_host: str                  = Field("0.0.0.0",                    env="API_HOST")
    api_port: int                  = Field(8000,                         env="API_PORT")

    model_config = SettingsConfigDict(
        env_file=".env",
        env_file_encoding="utf-8",
        extra="ignore",
    )

settings = Settings()
