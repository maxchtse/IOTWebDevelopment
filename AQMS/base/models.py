from django.db import models

# Create your models here.

class SensorReading(models.Model):
    temperature = models.IntegerField()
    humidity = models.IntegerField()
    airPressure = models.IntegerField()
    co = models.IntegerField()
    co2 = models.IntegerField()
    mox = models.IntegerField()
    ch4 = models.IntegerField()
    no2 = models.IntegerField()

    def __str__(self) -> str:
        return self.name