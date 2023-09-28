#include <WiFi.h>
#include <HTTPClient.h>
#include "Adafruit_CCS811.h"

// =============================
// ==== Configurable Constants ====
// =============================
const char* ssid = "";
const char* password = "";
const char* firebaseUrl = "https://cits5506-default-rtdb.firebaseio.com/sensorData.json";
#define BUZZZER_PIN 32
#define COV_RATIO 0.2 // ug/mmm / mv for dust sensor
#define NO_DUST_VOLTAGE 10 // Reduced from 400 for dust sensor testing 
#define SYS_VOLTAGE 3300 // unit: mV for dust sensor
const int iled = 23; //drive the led of dust sensor
const int vout = 33; //analog input for dust sensor
struct SensorThresholds {
    int densityThreshold;
    int eCO2Threshold;
    int TVOCThreshold;
};
SensorThresholds thresholds = {
    .densityThreshold = 50,
    .eCO2Threshold = 800,
    .TVOCThreshold = 300,
};

// ==========================
// ==== Buzzer Constants ====
// ==========================
#include "BuzzerNotes.h"

int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int noteDurations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

void playMelody() {
  int size = sizeof(noteDurations) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
  }
}

// ==========================
// ====   Dust Sensor   ====
// ==========================
float density, voltage;
int   adcvalue;

int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;
    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}

// ===================================
// ==== CO2 Sensor Initialization ====
// ===================================
Adafruit_CCS811 ccs;
// GPIO 21 for SDA (Default) 
// GPIO 22 for SCL (Default)

// ====================
// ==== Setup Logic ====
// ====================
void setup() {
    Serial.begin(9600);
    // Dust Sensor
    pinMode(iled, OUTPUT);
    digitalWrite(iled, LOW);
    // CO2 Sensor
    if (!ccs.begin()) {
        Serial.println("Failed to start CCS811!");
        while (1);
    }
    // Calibrate CO2 Sensor: temperature offset
    while(!ccs.available());
    float temp = ccs.calculateTemperature();
    ccs.setTempOffset(temp - 25.0);
    // WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

// =====================
// ==== Main Looping ====
// =====================
void loop() {
    // Dust Sensor
    digitalWrite(iled, HIGH); 
    delayMicroseconds(280); 
    adcvalue = analogRead(vout); 
    digitalWrite(iled, LOW); 
    adcvalue = Filter(adcvalue); 
    voltage = (SYS_VOLTAGE / 4095.0) * adcvalue * 11; 
    if (voltage >= NO_DUST_VOLTAGE) { 
        voltage -= NO_DUST_VOLTAGE; 
        density = voltage * COV_RATIO; 
    } else { 
        density = 0; 
    }
    Serial.printf("The current dust concentration is: %f ug/m3\n", density);
    if (density > thresholds.densityThreshold) playMelody();

    // CO2 Sensor
    int eCO2 = 0, TVOC = 0;
    if (ccs.available()) {
        float temp = ccs.calculateTemperature();
        if (!ccs.readData()) {
            eCO2 = ccs.geteCO2();
            TVOC = ccs.getTVOC();
            Serial.printf("CO2: %d ppm\n", eCO2);
            Serial.printf("TVOC: %d\n", TVOC);
            if (eCO2 > thresholds.eCO2Threshold) playMelody();
            if (TVOC > thresholds.TVOCThreshold) playMelody();
        } else {
            Serial.println("Error reading CCS811 data!");
        }
    }

    // Send data to Firebase
    String jsonData = "{\"eCO2\":" + String(eCO2) + ",\"TVOC\":" + String(TVOC) + ",\"DustDensity\":" + String(density) + "}";
    HTTPClient http;
    http.begin(firebaseUrl);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode == 200) {
        Serial.println("Data sent to Firebase!");
    } else {
        Serial.println("Error sending data: ");
        Serial.println(httpResponseCode);
    }
    http.end();
    delay(1000);
}