from django.shortcuts import render
from .models import SensorReading

# Create your views here.

def home(request):
    gases = SensorReading.objects.all()
    context = {'gases': gases}
    return render(request, 'base/home.html', context)

def sensor_data(request):
    data = SensorReading.objects.all()
    return render(request, 'sensor_data.html', {'data': data})