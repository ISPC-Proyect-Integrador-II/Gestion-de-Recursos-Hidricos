#!/usr/bin/env bash
# Configura estas variables:
DOMAIN="hidroponiatest"                                # tu subdominio
TOKEN="550f66d9-2adf-4c3e-82c3-6f39900838f2"    # tu token de DuckDNS
# Llamada a DuckDNS para actualizar IP
curl -s "https://www.duckdns.org/update?domains=$DOMAIN&token=$TOKEN&ip=" \
     | grep -q "OK" && exit 0 || exit 1
