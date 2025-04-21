import os
from pathlib import Path
import environ

# ------------------------------------
# 1. Base de proyecto y carga de .env
# ------------------------------------
BASE_DIR = Path(__file__).resolve().parent.parent
env = environ.Env(
    # casting por defecto
    DEBUG=(bool, False),
)

# Lee .env en BASE_DIR
environ.Env.read_env(os.path.join(BASE_DIR, '.env'))

# --------------------
# 2. Seguridad & hosts
# --------------------
SECRET_KEY = env('SECRET_KEY')
DEBUG = env('DEBUG')
ALLOWED_HOSTS = env.list('ALLOWED_HOSTS', default=[])

# --------------------------------
# 3. Rutas de templates y apps
# --------------------------------
INSTALLED_APPS = [
    # ...
    'django.contrib.staticfiles',
    'frontend',
    # (no incluimos aún apps de base de datos ni auth)
]

TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        # Apunta a tu carpeta de templates
        'DIRS': [BASE_DIR / 'frontend' / 'templates'],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                # procesadores por defecto…
                'django.template.context_processors.static',
                # …
            ],
        },
    },
]

ROOT_URLCONF = 'dashboard.urls'
WSGI_APPLICATION = 'dashboard.wsgi.application'
ASGI_APPLICATION = 'dashboard.asgi.application'

# -----------------------------
# 4. Configuración de estáticos
# -----------------------------
# URL para referenciar en templates: e.g. <link href="{% static 'css/main.css' %}">
STATIC_URL = env('STATIC_URL')

# Carpeta donde Django buscará tus archivos estáticos en desarrollo
STATICFILES_DIRS = [
    BASE_DIR / 'frontend' / 'static',
]

# Carpeta destino al hacer collectstatic (esto es lo que luego subirás a GH Pages)
STATIC_ROOT = BASE_DIR / 'docs' / 'static'
