
/*
    Purpose:
        Main function for Greenhouse control.

    Dependencies:
        DHT library
        Set of custom built greenhouse libraries

    Assumptions/Limitations:
        None
        
    Modification History:
        10/2015 - Initial version
        10/2015 - Added fan control

*/


// Libraries
#include <dht.h>
#include <readMoisture.h>
#include <readTempHum.h>
#include <timer.h>
#include <effector.h>

// Constants
#define MOISTURE_INTERVAL 20    // Interval (s) to run moisture check
#define TEMPHUM_INTERVAL  10    // Interval (s) to run temperature/humidity check
#define FAN_TIMER 300           // Time (s) to run the fan

// Calibration data
const int moistMin = 300; // Trigger value to turn on irrigation
const int moistMax = 600; // Trigger value to turn off irrigation
const int fanTempMax = 75; // Trigger temperature value to turn on fan
const int fanTempMin = 50; // Trigger temperature value to turn off fan
const int fanHumMax = 50; // Trigger humidity value to turn on fan
const int fanHumMin = 40; // Trigger humidiy value to turn off fan
const int ventTempMax = 60; // Trigger temperature value to open vents
const int ventTempMin = 50; // Trigger temperature value to close vents
const int ventHumMax = 50; // Trigger humidity value to open vents
const int ventHumMin = 40; // Trigger humidiy value to close vents
bool fanPolarity = 0; // The fan is connected through a relay which is active with a low signal

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
bool fanStatus;

// Define timers
Timer tempHumTimer;
Timer moistureTimer;
Timer fanTimer;

// Define Effectors
Effector fan(fanPinD, fanPolarity);

void setup() {
    // Open a serial connection
    Serial.begin(9600);
    lastTime = millis();

    // Setup the timers
    tempHumTimer.set_threshold(1.0 / TEMPHUM_INTERVAL);
    moistureTimer.set_threshold(1.0 / MOISTURE_INTERVAL);
    fanTimer.set_threshold(1.0 / FAN_TIMER);
}

void loop() {  

    // Compute time interval since this code was last run
    currentTime = millis();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Increment the timers
    tempHumTimer.increment_timer(deltaTime);
    moistureTimer.increment_timer(deltaTime);
    fanTimer.increment_timer(deltaTime);
    
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

    // If the fan is not currently running and the temperature
    // exceeds the preset value, turn on the fan
    if (!fan.get_status() && fTemperatureVal > fanTempMax) {
        fan.start_digital(fanPinD);
        fan.set_status(true);
        fanTimer.reset_timer();       
    }
    // If the fan is on and the timer is expired
    if (fan.get_status() && fanTimer.evaluate_timer()) {
        fan.stop_digital(fanPinD); 
        fan.set_status(false);
    }
    

    
    // Delay
    //delay(2000);

  
}
