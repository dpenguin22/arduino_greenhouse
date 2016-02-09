/*
    Purpose:
        Function for reading temerature/humidity sensor

    Dependencies:
        DHT library
        Arduino library

    Assumptions/Limitations:
        Only tested with DHT22 sensor

    Modification History:
        10/2015 - Initial version
	02/2016 - Added check on return value

*/

// Libraries
#include <dht.h>
#include <arduino.h>

//Constants
dht DHT;

int readTempHum(int dhtPin, float *ftemperature, float *humidity)
{
    // Variables
    int chk;
    float temperature;  // Stores temperature value

    // Read data from DHT22 sensor
    // Check data pin and read values
    chk = DHT.read22(dhtPin); 

    //Read data and store it to variables hum and temp
    if (chk == 0) {
        *humidity = DHT.humidity;
        temperature= fabs(DHT.temperature);
        *ftemperature = temperature * 9 / 5 + 32;
    } 
}  	 
