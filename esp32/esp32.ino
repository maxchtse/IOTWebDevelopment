#include <WiFi.h>
#include <HTTPClient.h>
#include "Adafruit_CCS811.h"
#include <Arduino.h>
#include <time.h>
char timestamp[64] = ""; 

// =============================
// ==== Configurable Constants ====
// =============================
const char* ssid = "";
const char* password = "";
const char* firebaseUrl = "https://cits5506-default-rtdb.firebaseio.com/sensorData.json";
#define BUZZZER_PIN 23
#define COV_RATIO 0.2 // ug/mmm / mv for dust sensor
#define NO_DUST_VOLTAGE 10 // Reduced from 400 for dust sensor testing 
#define SYS_VOLTAGE 3300 // unit: mV for dust sensor
const int iled = 32; //drive the led of dust sensor
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

// Function to play a melody
void playMelody() {
  // Calculate the number of notes in the melody
  int size = sizeof(noteDurations) / sizeof(int);

  // Loop through each note in the melody
  for (int thisNote = 0; thisNote < size; thisNote++) {
    
    // Calculate the duration of each note in milliseconds
    int noteDuration = 1000 / noteDurations[thisNote];
    
    // Play the note using the tone() function
    // BUZZZER_PIN is the pin number where the buzzer is connected
    // melody[thisNote] is the frequency of the note to be played
    // noteDuration is the duration for which the note will be played
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);
    
    // Calculate the pause duration between notes. It's 130% of the note duration.
    int pauseBetweenNotes = noteDuration * 1.30;
    
    // Pause for the calculated duration
    delay(pauseBetweenNotes);
    
    // Stop playing the tone
    noTone(BUZZZER_PIN);
  }
}


// ==========================
// ====   Dust Sensor   ====
// ==========================
float density, voltage;  // Variables to store density and voltage values
int   adcvalue;          // Variable to store ADC value

// Function to filter the ADC values using a simple moving average filter
int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;  // Static variables: flag to check first run, buffer to store last 10 values, sum of the buffer
  const int _buff_max = 10;                   // Constant to define the maximum size of the buffer
  int i;                                      // Loop counter
  
  // Check if it's the first time the function is called
  if(flag_first == 0)
  {
    flag_first = 1;                           // Set the flag to indicate that the function has been called before
    for(i = 0, sum = 0; i < _buff_max; i++)   // Initialize the buffer with the current value and calculate the sum
    {
      _buff[i] = m;                           // Set each buffer value to the current input
      sum += _buff[i];                        // Add the current buffer value to the sum
    }
    return m;                                 // Return the current input value
  }
  else
  {
    sum -= _buff[0];                          // Subtract the oldest value from the sum
    for(i = 0; i < (_buff_max - 1); i++)      // Shift all buffer values to the left
    {
      _buff[i] = _buff[i + 1];                // Move each buffer value one position to the left
    }
    _buff[9] = m;                             // Add the current input value to the end of the buffer
    sum += _buff[9];                          // Add the current input value to the sum
    
    i = sum / 10.0;                           // Calculate the average of the buffer values
    return i;                                 // Return the average value
  }
}


// ===================================
// ==== CO2 Sensor Initialization ====
// ===================================
Adafruit_CCS811 ccs;
// GPIO 21 for SDA (Default) 
// GPIO 22 for SCL (Default)

// ===================================
// === BME280 Sensor Initialization ===
// ===================================
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

#define SEALEVELPRESSURE_HPA (1018.8) // Barometric Pressure in Perth, AU

Adafruit_BME280 bme;

// ===================================
// === Gas Sensor Initialization ===
// ===================================
String mode;
int CH4, NO, CO;
float NO2;
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
  // Seed for random number generator
  randomSeed(analogRead(0));
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

  // config time
  configTime(8 * 3600, 8 * 3600, "pool.ntp.org", "time.nist.gov"); // UTC+8
  
  // BME280 Sensor
  bool status;
  status = bme.begin(0x76);  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
}

// =====================
// ==== Main Looping ====
// =====================
void loop() {
  // Reading data from BME sensor
  float Temp = bme.readTemperature();  // Read temperature in Celsius
  float hPa = bme.readPressure() / 100.0F;  // Read pressure in hPa
  float Altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);  // Read altitude in meters
  float Humidity = bme.readHumidity();  // Read humidity in percentage

  // Print BME sensor readings to Serial
  Serial.print("Temperature: ");
  Serial.print(Temp);
  Serial.println(" °C");

  Serial.print("Pressure: ");
  Serial.print(hPa);
  Serial.println(" hPa");

  Serial.print("Altitude: ");
  Serial.print(Altitude);
  Serial.println(" meters");

  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.println(" %");

  // Dust Sensor readings
  digitalWrite(iled, HIGH);  // Turn on infrared LED
  delayMicroseconds(280);  // Short delay
  adcvalue = analogRead(vout);  // Read ADC value from dust sensor
  digitalWrite(iled, LOW);  // Turn off infrared LED
  adcvalue = Filter(adcvalue);  // Filter the ADC value
  voltage = (SYS_VOLTAGE / 4095.0) * adcvalue * 11;  // Convert ADC value to voltage
  if (voltage >= NO_DUST_VOLTAGE) {  // If voltage is above threshold
      voltage -= NO_DUST_VOLTAGE;  // Adjust voltage
      density = voltage * COV_RATIO;  // Calculate dust density
  } else {  // If voltage is below threshold
      density = 0;  // Set density to zero
  }
  // Print dust density to Serial
  Serial.printf("The current dust concentration is: %f ug/m3\n", density);
  if (density > thresholds.densityThreshold) playMelody();  // Play melody if density exceeds threshold

  // CO2 Sensor readings
  int eCO2 = 0, TVOC = 0;  // Initialize eCO2 and TVOC variables
  if (ccs.available()) {  // If CCS811 sensor data is available
      float temp = ccs.calculateTemperature();  // Calculate temperature (unused in this code)
      if (!ccs.readData()) {  // If data reading is successful
          eCO2 = ccs.geteCO2();  // Get eCO2 value
          TVOC = ccs.getTVOC();  // Get TVOC value
          // Print eCO2 and TVOC values to Serial
          Serial.printf("CO2: %d ppm\n", eCO2);
          Serial.printf("TVOC: %d\n", TVOC);
          if (eCO2 > thresholds.eCO2Threshold) playMelody();  // Play melody if eCO2 exceeds threshold
          if (TVOC > thresholds.TVOCThreshold) playMelody();  // Play melody if TVOC exceeds threshold
      } else {  // If data reading fails
          Serial.println("Error reading CCS811 data!");  // Print error message
      }
  }

  // Get current timestamp
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){  // If getting local time fails
    Serial.println("Failed to obtain time");  // Print error message
  }
  else {
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);  // Format time into a string
    Serial.println(timestamp);  // Print timestamp to Serial
  }
  
  // Stimulate gas sensors (function not provided, so purpose is assumed)
  stimulate_sensors();

  // Prepare data to send to Firebase
  String jsonData = "{"
    "\"timestamp\":\"" + String(timestamp) + "\","
    // ... (other data fields) ...
    "\"DustDensity\":" + String(density) +
  "}";

  // Send data to Firebase
  HTTPClient http;
  http.begin(firebaseUrl);  // Start HTTP connection to Firebase URL
  http.addHeader("Content-Type", "application/json");  // Set content type header
  int httpResponseCode = http.POST(jsonData);  // Send POST request with JSON data
  if (httpResponseCode == 200) {  // If response code is 200 (OK)
      Serial.println("Data sent to Firebase!");  // Print success message
  } else {  // If response code is not 200
      Serial.println("Error sending data: ");  // Print error message
      Serial.println(httpResponseCode);  // Print HTTP response code
  }
  http.end();  // End HTTP connection
  delay(1000);  // Delay for 1 second before next loop iteration
}


// Function to simulate sensor readings for testing purposes
void stimulate_sensors() {
  int choice = random(1, 5);  // Randomly choose a number between 1 and 4
  
  // Set the mode based on the random choice
  if (choice == 1) mode = "danger";
  else mode = "safe";

  // If the mode is "safe", generate random values within safe ranges
  if (mode == "safe") {
    CH4 = random(0, 10001);  // Methane in ppm
    NO = random(0, 26);  // Nitric Oxide in ppm
    NO2 = random(0, 501) / 1000.0;  // Nitrogen Dioxide in ppm
    CO = random(0, 10);  // Carbon Monoxide in ppm

    // Print the simulated safe range values to Serial
    Serial.println("Safe Ranges:");
    Serial.print("CH4 (Methane): "); Serial.print(CH4); Serial.println(" ppm");
    Serial.print("NO (Nitric Oxide): "); Serial.print(NO); Serial.println(" ppm");
    Serial.print("NO2 (Nitrogen Dioxide): "); Serial.print(NO2); Serial.println(" ppm");
    Serial.print("CO (Carbon Monoxide): "); Serial.print(CO); Serial.println(" ppm");
  } 
  // If the mode is "danger", generate random values within dangerous ranges
  else if (mode == "danger") {
    CH4 = random(10001, 100001);  // Methane in ppm
    NO = random(26, 101);  // Nitric Oxide in ppm
    NO2 = random(501, 10001) / 1000.0;  // Nitrogen Dioxide in ppm
    CO = random(10, 101);  // Carbon Monoxide in ppm
    
    // Print the simulated danger range values to Serial
    Serial.println("Danger Ranges:");
    Serial.print("CH4 (Methane): "); Serial.print(CH4); Serial.println(" ppm");
    Serial.print("NO (Nitric Oxide): "); Serial.print(NO); Serial.println(" ppm");
    Serial.print("NO2 (Nitrogen Dioxide): "); Serial.print(NO2); Serial.println(" ppm");
    Serial.print("CO (Carbon Monoxide): "); Serial.print(CO); Serial.println(" ppm");

    // Play a melody to indicate danger
    playMelody();
  } 
  // If the mode is neither "safe" nor "danger", print an error message
  else {
    Serial.println("Invalid input. Please enter 'safe' or 'danger'.");
  }
}
