from django.shortcuts import render
from .models import SensorReading

# Create your views here.

def home(request):
    gases = SensorReading.objects.all()
    return render(request, 'base/home.html', {'gases': gases})