/*
    Purpose:
        Function for reading moisture sensor

    Dependencies:
        Arduino library

    Assumptions/Limitations:
        Only tested with Arduino compatible High Sensitivity Moisture Sensor

    Modification History:
        10/2015 - Initial version

*/

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
