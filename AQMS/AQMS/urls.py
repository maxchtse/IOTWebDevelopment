from django.contrib import admin
from django.urls import path, include
from base import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('base.urls')),
    path('download-json/', views.download_json, name='download-json'),
]
