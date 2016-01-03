
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
        11/2015 - Added serial read function
        11/2015 - Added vent control and effector commands
        12/2015 - Add softsare serial ports for comm with esp8266

*/


// Libraries
#include <dht.h>
#include <readMoisture.h>
#include <readTempHum.h>
#include <readSerialMessage.h>
#include <timer.h>
#include <effector.h>
#include <SoftwareSerial.h>
#include <keys.h>


// Constants
#define MAX_MESSAGE_SIZE  120
#define MOISTURE_INTERVAL 20    // Interval (s) to run moisture check
#define TEMPHUM_INTERVAL  10    // Interval (s) to run temperature/humidity check
#define FAN_TIMER 300           // Time (s) to run the fan
#define VENT_TIMER 10           // Time (s) to actuate the open/close of the vent
#define POST_TIMER 1800         // Time (s) to post sensor data to web

// Commands
bool runFanCMD = 0;
bool openVentCMD = 0;
bool closeVentCMD = 0;
bool runPumpCMD = 0;

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
bool fanPolarity = 0;  // The fan is connected through a relay which is active with a low signal
bool ventPolarity = 0; // The vent is connected through a relay which is active with a low signal

// Analog pins
const int moistPinA = 5; // Data pin of moisture sensor to Arduino analog pin 5

// Digital pins
SoftwareSerial espSerial(2,3);  // Rx, Tx
const int pumpPinD = 4;  // Switch on water pump 
const int fanPinD  = 5;  // Switch on fan
const int dhtPinD  = 6;  // Data pin of DHT-22
const int moistPowerPinD = 7; // Apply power to the moisture sensor
const int ventOpenPinD = 8;  // Open vents
const int ventClosePinD = 9; // Close vents

// Variables
unsigned long lastTime;
unsigned long deltaTime;
unsigned long currentTime;
int messageSize;
float moistVal;
float fTemperatureVal;
float humidityVal;
bool fanStatus;
bool ventPosition;
String incomingString = "";
String postString = "";

// Define timers
Timer tempHumTimer;
Timer moistureTimer;
Timer fanTimer;
Timer ventTimer;
Timer postTimer;

// Define Effectors
Effector fan(fanPinD, fanPolarity);
Effector ventOpen(ventOpenPinD, ventPolarity);
Effector ventClose(ventClosePinD, ventPolarity);


void setup() {
    // Open a serial connection
    Serial.begin(112500);
    espSerial.begin(112500);
    lastTime = millis();

    // Setup the timers
    tempHumTimer.set_threshold(1.0 / TEMPHUM_INTERVAL);
    moistureTimer.set_threshold(1.0 / MOISTURE_INTERVAL);
    fanTimer.set_threshold(1.0 / FAN_TIMER);
    ventTimer.set_threshold(1.0 / VENT_TIMER);
    postTimer.set_threshold(1.0 / POST_TIMER);

    // Initialize effectors to the off state
    digitalWrite(fanPinD, HIGH);
    digitalWrite(ventOpenPinD, HIGH);
    digitalWrite(ventClosePinD, HIGH);

    // Initialize power to moisture sensors off
    digitalWrite(moistPowerPinD, LOW);
  
}

void loop() { 
    char myString[MAX_MESSAGE_SIZE] = "";  // Storage for string to send across Serial
     
    // Compute time interval since this code was last run
    currentTime = millis();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
        
    // Increment the timers
    tempHumTimer.increment_timer(deltaTime);
    moistureTimer.increment_timer(deltaTime);
    fanTimer.increment_timer(deltaTime);
    ventTimer.increment_timer(deltaTime);
    
    // Process commands
    if (espSerial.available() > 0) {
            messageSize = read_serial_message(espSerial, &incomingString, MAX_MESSAGE_SIZE);

            //Serial.print("Message size is: ");
            //Serial.println(messageSize);
            Serial.print("Message is: ");
            Serial.println(incomingString);

            if (incomingString == "runFan") {
                Serial.println("Received the run fan command.");
                runFanCMD = 1;
            } else if (incomingString == "openVent") {
                openVentCMD = 1;
            } else if (incomingString == "closeVent") {
                closeVentCMD = 1;
            }
            
    }

    // Execute commands
    if (runFanCMD) {
        // Start the fan
        fan.start_digital(fanPinD);
        fan.set_status(true);
        fanTimer.reset_timer();
        // Reset the command
        runFanCMD = 0;
    } else if (openVentCMD) {
        ventOpen.start_digital(ventOpenPinD);
        ventOpen.set_status(true);
        ventTimer.reset_timer();
        Serial.println("Received the open vent command.");
        // Reset the command
        openVentCMD = 0;
    } else if (closeVentCMD) {
        ventClose.start_digital(ventClosePinD);
        ventClose.set_status(true);
        ventTimer.reset_timer();
        Serial.println("Received the close vent command.");
        // Reset the command
        closeVentCMD = 0;
    }

    
    // Collect seonsor data
    
    // Call function to read moisture data
    if (moistureTimer.evaluate_timer()) {

        // Apply power to the sensor. There is a delay
        // in the read function that will pause before
        // taking the first reading
        digitalWrite(moistPowerPinD, HIGH);
        
        moistVal = readMoisture(moistPinA);
        //Serial.print("Moisture: ");
        //Serial.println(moistVal);
    }
 
    // Call function to read temperature and humidity data
    if (tempHumTimer.evaluate_timer()) {
        readTempHum(dhtPinD, &fTemperatureVal, &humidityVal);
        
        sprintf(myString, "Temperature: %d degF. Humidity: %d%%. Moisture: %d. Fan Status %d. Vent position: %d\n", (int) fTemperatureVal, (int) humidityVal, (int) moistVal, (int) ventPosition, (int) fan.get_status());
        
        Serial.print(myString);
        espSerial.print(myString);
        espSerial.flush();   // Don't allow the next read until this data is finished transmitting

    }

    // Process effectors

    // If the fan is not currently running and the temperature
    // exceeds the preset value, turn on the fan
    
    if (!fan.get_status() && fanTimer.evaluate_timer() && fTemperatureVal > fanTempMax) {
        
        fan.start_digital(fanPinD);
        fan.set_status(true);
        fanTimer.reset_timer();      
    }
    // If the fan is on and the timer is expired
    if (fan.get_status() && fanTimer.evaluate_timer()) {
        fan.stop_digital(fanPinD); 
        fan.set_status(false);
        fanTimer.reset_timer();
    }

//    if (ventOpen.get_status()) Serial.print(".");
//    if (ventClose.get_status()) Serial.print(".");
    // If the vent is opening and the timer has expired
    if (ventOpen.get_status() && ventTimer.evaluate_timer()) {
        ventOpen.stop_digital(ventOpenPinD);
        ventOpen.set_status(false);
        ventTimer.reset_timer();
        Serial.println("Vent has completed opening.");
        ventPosition = 1;
    }

    // If the vent is closing and the timer has expired
    if (ventClose.get_status() && ventTimer.evaluate_timer()) {
        ventClose.stop_digital(ventClosePinD);
        ventClose.set_status(false);
        ventTimer.reset_timer();
        Serial.println("Vent has completed closing.");
        ventPosition = 0;
    }

    // Post data to web server
    if (postTimer.evaluate_timer()) {
        
        String postStr = THINGSPEAK_APIKEY;
        postStr +="&field1=";
        postStr += String(fTemperatureVal);
        postStr +="&field2=";
        postStr += String(humidityVal);
        postStr += "&field3=";
        postStr += String(moistVal);
        postStr += "\r\n\r\n";

        // Send data to esp8266 so it can post the data
        espSerial.print(postStr);
       
    }

  
}
