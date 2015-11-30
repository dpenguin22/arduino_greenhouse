/*
    Purpose:
        Function for setting up the CC3000 Wifi shield

    Dependencies:
        Adafruit CC3000 library

    Assumptions/Limitations:

    Modification History:
        11/2015 - Initial version

*/

// Libraries
#include "setupWifi.h"


void initialize_wifi()
{

    // Initialize the module
    cc3000.begin();

    // Connect to the WiFi network defined in the header file
    Serial.print("\nAttempting to connect to: ");
    Serial.println(WiFiSSID);
    if (!cc3000.connectToAP(WiFiSSID, WiFiPSK, WiFiSECURITY)) {
        Serial.println(F("Failed!"));
        while(1);
    }
    Serial.println("Connected!");

    Serial.println(F("Requesting DHCP"));
    while (!cc3000.checkDHCP())
    {
        delay(100); // ToDo: Insert a DHCP timeout!
    }  

    // Display the IP address DNS, Gateway, etc.
    while (! displayConnectionDetails(cc3000)) {
        delay(1000);
    }

    // Start the server
    server.begin();
    Serial.println("Server started"); 
    Serial.println(F("Listening for connections..."));


}

// Tries to read the IP address and other connection details
bool displayConnectionDetails(Adafruit_CC3000 cc3000)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

