/*
    Purpose:
        Prototype and variable definition for setting up the CC3000 Wifi shield

    Dependencies:
        Adafruit CC3000 library
        SPI library

    Assumptions/Limitations:
        None

    Modification History:
        11/2015 - Initial version

*/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

#ifndef SETUPWIFI_H
#define SETUPWIFI_H

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "Barthnet_RE";
const char WiFiPSK[] = "thepiggiesmustnotbedisturbed";
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WiFiSECURITY   WLAN_SEC_WPA2


// Declaration of the cc3000 class
extern Adafruit_CC3000 cc3000;

// Declaration fo the server class
extern Adafruit_CC3000_Server server;

#define TIMEOUT_MS 500    // Amount of time in milliseconds to wait for an incoming request to finish.

// Functioon prototype
void initialize_wifi();
bool displayConnectionDetails(Adafruit_CC3000 cc3000);
void process_wifi();

#endif
