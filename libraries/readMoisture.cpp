// Function to read data from the soil moisture sensor 

// Libraries
#include <arduino.h>

float readMoisture(int moistPin)
{
    // Variables
    float moistVal;

    // Read data from moisture sensor
    moistVal = analogRead(moistPin);

    return moistVal;
}  	 
