from django.urls import path
from .views import index, metrics

urlpatterns = [
    path('', index, name='index'),
    path('metrics/', metrics, name='metrics'),
]
