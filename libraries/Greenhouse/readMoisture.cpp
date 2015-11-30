/*
    Purpose:
        Function for reading moisture sensor

    Dependencies:
        Arduino library

    Assumptions/Limitations:
        Only tested with Arduino compatible High Sensitivity Moisture Sensor

    Modification History:
        10/2015 - Initial version
        11/2015 - Average five measurements

*/

// Libraries
#include <arduino.h>

float readMoisture(int moistPin)
{
    // Variables
    int nSamp = 0;
    float moistVal = 0;
    float moistSum = 0;

    for (int i = 0; i <5; i++) {
        // Wait 1 second. This will pause the entire controller
        delay(1000);

        // Read data from moisture sensor
        moistVal = analogRead(moistPin);
        
        // Only accept samples above a threshold, anything lower
        // would be an error or misplaced sensor
        if (moistVal > 50) {
            nSamp +=1;
            moistSum += moistVal;
        }
    }
    
    // Average the samples
    if (nSamp > 0) {
       return (moistSum / nSamp);
    } else {
        return 0.0;
    }
}  	 
