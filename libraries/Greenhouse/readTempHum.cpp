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

*/

// Libraries
#include <dht.h>
#include <arduino.h>

//Constants
dht DHT;

void readTempHum(int dhtPin, float *ftemperature, float *humidity)
{
    // Variables
    int chk;
    float temperature;  // Stores temperature value

    // Read data from DHT22 sensor
    // Check data pin and read values
    chk = DHT.read22(dhtPin); 

    //Read data and store it to variables hum and temp
    *humidity = DHT.humidity;
    temperature= DHT.temperature;
    *ftemperature = temperature * 9 / 5 + 32;

}  	 
