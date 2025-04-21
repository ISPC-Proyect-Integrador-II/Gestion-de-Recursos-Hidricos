from django.shortcuts import render

def index(request):
    """
    Vista que renderiza la página principal del dashboard.
    """
    return render(request, 'index.html')
