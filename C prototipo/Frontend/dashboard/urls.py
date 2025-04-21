from django.contrib import admin
from django.urls import path, include
from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    # Descomenta si quieres usar el admin:
    # path('admin/', admin.site.urls),

    # Todas las URLs de frontend
    path('', include('frontend.urls')),
]

# En DEBUG, servir archivos estáticos desde STATICFILES_DIRS[0]
if settings.DEBUG:
    urlpatterns += static(
        settings.STATIC_URL,
        document_root=settings.STATICFILES_DIRS[0]
    )
