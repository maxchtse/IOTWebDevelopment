from django.shortcuts import render
from django.http import JsonResponse
from .models import SensorReading

# Create your views here.

def home(request):
    gases = SensorReading.objects.all()
    return render(request, 'base/home.html', {'gases': gases})

def download_json(request):
    # Retrieve data from the database (you can filter as needed)
    data_objects = SensorReading.objects.all()

    # Serialize the data to JSON
    data_list = [{"temperature": obj.temperature, 
                  "humidity": obj.humidity,
                  "airPressure": obj.airPressure,
                  "co": obj.co,
                  "co2": obj.co2,
                  "mox":obj.mox,
                  "ch4": obj.ch4,
                  "no2": obj.no2,
                  "data": obj.data_json} for obj in data_objects]

    # Create a JSON response with the serialized data
    response_data = {"data": data_list}

    # You can customize the response headers as needed
    response = JsonResponse(response_data, json_dumps_params={'indent': 9})
    response['Content-Disposition'] = 'attachment; filename="data.json"'

    return response