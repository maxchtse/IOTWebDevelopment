from django.urls import path
from . import views

urlpatterns = [
    path('', views.home, name='home'),
    path('sensor_data/', views.sensor_data, name='sensor_data'),
]