from django.shortcuts import render

def index(request):
    return render(request, 'index.html')

def metrics(request):
    return render(request, 'metrics.html')
