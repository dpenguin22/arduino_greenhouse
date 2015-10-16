
// Main function for Greenhouse control


// Libraries
#include <dht.h>
#include <readMoisture.h>
#include <readTempHum.h>
#include <timer.h>

// Constants
#define MOISTURE_INTERVAL 20    // Interval (s) to run moisture check
#define TEMPHUM_INTERVAL  10    // Interval (s) to run temperature/humidity check

// Calibration data
const int moistMin = 300; // Trigger value to turn on irrigation
const int moistMax = 600; // Trigger value to turn off irrigation
const int fanTempMax = 60; // Trigger temperature value to turn on fan
const int fanTempMin = 50; // Trigger temperature value to turn off fan
const int fanHumMax = 50; // Trigger humidity value to turn on fan
const int fanHumMin = 40; // Trigger humidiy value to turn off fan
const int ventTempMax = 60; // Trigger temperature value to open vents
const int ventTempMin = 50; // Trigger temperature value to close vents
const int ventHumMax = 50; // Trigger humidity value to open vents
const int ventHumMin = 40; // Trigger humidiy value to close vents

// Analog pins
const int moistPinA = 5; // Data pin of moisture sensor to Arduino analog pin 5

// Digital pins
const int dhtPinD  = 2;  // Data pin of DHT-22 to Arduino digital pin 2
const int pumpPinD = 3;  // Switch on water pump 
const int fanPinD  = 4;  // Switch on fan
const int ventPinD = 5;  // Open vents


// Variables
unsigned long lastTime;
unsigned long deltaTime;
unsigned long currentTime;
float moistVal;
float fTemperatureVal;
float humidityVal;

// Define timers
Timer tempHumTimer;
Timer moistureTimer;

void setup() {
    // Open a serial connection
    Serial.begin(9600);
    lastTime = millis();

    tempHumTimer.set_threshold(1.0 / TEMPHUM_INTERVAL);
    moistureTimer.set_threshold(1.0 / MOISTURE_INTERVAL);
    
}

void loop() {  

    // Compute time interval since this code was last run
    currentTime = millis();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    tempHumTimer.increment_timer(deltaTime);
    moistureTimer.increment_timer(deltaTime);
    
    Serial.print("tempHumTimer: ");
    Serial.print(tempHumTimer.get_value());
    Serial.print("  moistureTimer: ");
    Serial.println(moistureTimer.get_value());
    
    // Call function to read moisture data
    if (moistureTimer.evaluate_timer()) {
        moistVal = readMoisture(moistPinA);
        Serial.print("Moisture: ");
        Serial.println(moistVal);
    }
    
    // Call function to read temperature and humidity data
    if (tempHumTimer.evaluate_timer()) {
        readTempHum(dhtPinD, &fTemperatureVal, &humidityVal);
        Serial.print("Humidity: ");
        Serial.print(humidityVal);
        Serial.print(" %, Temp: ");
        Serial.print(fTemperatureVal);
        Serial.println("  Fahrenheit");
    }
    // Print out results
    

    
    // Delay
    //delay(2000);

  
}
