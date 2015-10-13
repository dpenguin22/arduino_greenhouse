#include <readMoisture.h>
#include <readTempHum.h>

// Main function for Greenhouse control


// Libraries
#include <dht.h>
#include <readMoisture.h>
#include <readTempHum.h>
#include <timer.h>

// Constants

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
float moistVal;
float fTemperatureVal;
float humidityVal;

void setup() {
    // Open a serial connection
    Serial.begin(9600);
}

void loop() {  
  
    // Call function to read moisture data
    moistVal = readMoisture(moistPinA);
    readTempHum(dhtPinD, &fTemperatureVal, &humidityVal);

    // Print out results
    Serial.print("Moisture: ");
    Serial.println(moistVal);
    Serial.print("Humidity: ");
    Serial.print(humidityVal);
    Serial.print(" %, Temp: ");
    Serial.print(fTemperatureVal);
    Serial.println("  Fahrenheit");
    
    // Delay
    delay(2000);

  
}
