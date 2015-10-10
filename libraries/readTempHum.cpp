// Function to read data from the DHT22 temperature and humidity sensor 

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
