
#define DEBUG 0

#include <ESP8266WiFi.h>
#include <string.h>
#include <dht.h>
//#include <SoftwareSerial.h>
#include <keys.h>

const int LEDPin = 0;
int read_message(String* message, int bufsize);
void updateThingSpeak(String tsData);

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
const int updateThingSpeakInterval = 600 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;

//////////////////////
// WiFi Definitions //
//////////////////////
WiFiClient client;

#define MAX_MESSAGE_SIZE 120

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {

    pinMode(LEDPin, OUTPUT);

    // Open a serial connection
    Serial.begin(112500);
    
    // Set WiFi mode to station (as opposed to AP or AP_STA)
    WiFi.mode(WIFI_STA);

    // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
    // to the stated [ssid], using the [passkey] as a WPA, WPA2,
    // or WEP passphrase.
    Serial.print("Connecting to WiFi network: ");
    Serial.println(WiFiSSID);
    
    WiFi.begin(WiFiSSID, WiFiPSK);

    // Set a static IP (optional)  
    IPAddress ip(192, 168, 1, 35);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);

    // Use the WiFi.status() function to check if the ESP8266
    // is connected to a WiFi network.
    while (WiFi.status() != WL_CONNECTED)
    {                   
        // Delays allow the ESP8266 to perform critical tasks
        // defined outside of the sketch. These tasks include
        // setting up, and maintaining, a WiFi connection.
        delay(500);
        Serial.print(".");
  
    }
    Serial.println("");
    Serial.print("Successfully connected to: ");
    Serial.println(WiFiSSID);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

   // Start the server
    server.begin();
    Serial.println("Server started"); 

}

void loop() {

    // Blink the LED to show the board is working
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);


    // Use the WiFi.status() function to check if the ESP8266
    // is connected to a WiFi network.

    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to re-establish connection to: ");
        Serial.println(WiFiSSID);
    }
    while (WiFi.status() != WL_CONNECTED)
    {                   
        WiFi.begin(WiFiSSID, WiFiPSK);
        // Delays allow the ESP8266 to perform critical tasks
        // defined outside of the sketch. These tasks include
        // setting up, and maintaining, a WiFi connection.
        delay(500);
        Serial.print(".");
    }
    
    // Check if a client has connected
    WiFiClient client2 = server.available();
    //if (!client2) {     
    //  return;
    //}

    String incomingString = "";   // Received serial message
    String req = "";
    String respMsg = "";          // HTTP Response message
    int messageSize;
    
    // Wait until the client sends some data
    if (DEBUG) {
        Serial.println("new client");
    }
    //while(!client2.available()){
    //  delay(1);
    //}
   
    // Read the first line of the request
    //req = client2.readStringUntil('\r');

    if (DEBUG) {
        Serial.print("The request: ");
        Serial.println(req);
    }
    client2.flush();

    // Match the request
    if (req.indexOf("/cmd/0") != -1) {
        respMsg = "Got signal 0";
        Serial.println("Got signal 0");
    } else if (req.indexOf("/cmd/1") != -1) {
        respMsg = "Got signal 1";
        Serial.println("Got signal 1");
    } else if (req.indexOf("/cmd/runFan") != -1) {
        respMsg = "Rcvd: Run Fan CMD";
        //Serial.println("runFan");
        Serial.write("runFan\n");
    } else if (req.indexOf("/cmd/postData") != -1) {
        Serial.write("postData\n");
        Serial.flush();
        delay(500);
        //Serial.write("");
        if (Serial.available() > 0) {
            messageSize = read_message(&incomingString, MAX_MESSAGE_SIZE);
            //Serial.print(F("Message size is: "));
            //Serial.println(messageSize);
            delay(500);
            if (incomingString.length() > 0) {
                respMsg = incomingString;   // assign to the response message
                
                // print the message received:
                //Serial.print(F("I received: "));
                //Serial.println(respMsg);
            } else {
                respMsg = "The string was empty";
                Serial.println(respMsg);
            }
        }
    } else if (req.indexOf("/cmd/status") != -1) {
        // send data only when you receive data:
        if (Serial.available() > 0) {
            messageSize = read_message(&incomingString, MAX_MESSAGE_SIZE);
            Serial.print("Message size is: ");
            Serial.println(messageSize);
            
            if (incomingString.length() > 0) {
                respMsg = incomingString;   // assign to the response message
                // print the message received:
                Serial.print("I received: ");
                Serial.println(respMsg);
            } else {
                respMsg = "The string was empty";
                Serial.println(respMsg);
            }
        }       
    } else {
        //respMsg = "Invalid request";
        //Serial.println("invalid request");
        //client2.stop();
        //return;
    }
  //Serial.print(F("Message size is: "));
  //Serial.println(messageSize);
  //Serial.print(F("I received: "));
  //Serial.println(respMsg);
  
    // Prepare the response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    if (respMsg.length() > 0) {
        s += respMsg;
    } else {
        s += "OK";
    } 
    s += "\n";

    //Serial.println(respMsg);
    // Send the response to the client
    //client.print(s);
    delay(100);
    if (DEBUG) {
        Serial.println("Client disconnected");
    }

    // Disconnect from ThingSpeak
    if (!client.connected() && lastConnected)
    {
        //Serial.println("...disconnected");
        //Serial.println();
    
        client.stop();
    }
    client.stop();
    // Update ThingSpeak
    if(!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval))
    {
        //Serial.write("postData\n");
        Serial.write('p');
        Serial.flush();
        //delay(500);
        //Serial.write("");
        if (Serial.available() > 0) {
            messageSize = read_message(&incomingString, MAX_MESSAGE_SIZE);
            //Serial.print(F("Message size is: "));
            //Serial.println(messageSize);
            //Serial.print("The message: ");
            //Serial.println(incomingString);
            delay(500);
            if (incomingString.length() > 0) {
                respMsg = incomingString;   // assign to the response message
                
                // print the message received:
                //Serial.print(F("I received: "));
                //Serial.println(respMsg);
                
            } else {
                respMsg = "The string was empty";
            }
        }
        //Serial.println(respMsg);
        //updateThingSpeak("field1="+analogValue0);
        //updateThingSpeak("field1="+String(4));
        //Serial.println("Testing post");
        //respMsg = "field1=66&field2=99&field3=200\r\n\r\n";
        updateThingSpeak(respMsg);
        //Serial.write("");
    }
  
    // Check if Arduino Ethernet needs to be restarted
    // if (failedCounter > 3 ) {startEthernet();}
  
    lastConnected = client.connected();
    
}

int read_message(String* message, int bufsize)
{
  String buffer = "";
  for (int index = 0; index < bufsize; index++) {

    // Wait until characters are available
    while (Serial.available() == 0) {
    }
    char ch = Serial.read(); // read next character
 
    //Serial.print(ch); // echo it back: useful with the serial monitor (optional)
    if (ch == '\n') {
      buffer += '\0';          // end of line reached: null terminate string
      *message = buffer;
      //Serial.println(*message);  // print full message before exiting
      return index;              // success: return length of string (zero if string is empty)
    }
    buffer += ch; // Append character to buffer
  }

  // Reached end of buffer, but have not seen the end-of-line yet.
  // Discard the rest of the line (safer than returning a partial line).

  *message = "The received message exceeded the buffer length";
  Serial.println(*message);
  return -1; // error: return negative one to indicate the input was too long
}


/*
 Arduino --> ThingSpeak Channel via Ethernet
 
 The ThingSpeak Client sketch is designed for the Arduino and Ethernet.
 This sketch updates a channel feed with an analog input reading via the
 ThingSpeak API (https://thingspeak.com/docs)
 using HTTP POST. The Arduino uses DHCP and DNS for a simpler network setup.
 The sketch also includes a Watchdog / Reset function to make sure the
 Arduino stays connected and/or regains connectivity after a network outage.
 Use the Serial Monitor on the Arduino IDE to see verbose network feedback
 and ThingSpeak connectivity status.
 
 Getting Started with ThingSpeak:
 
   * Sign Up for New User Account - https://thingspeak.com/users/new
   * Create a new Channel by selecting Channels and then Create New Channel
   * Enter the Write API Key in this sketch under "ThingSpeak Settings"
 
 Arduino Requirements:
 
   * Arduino with Ethernet Shield or Arduino Ethernet
   * Arduino 1.0+ IDE
   
  Network Requirements:
   * Ethernet port on Router    
   * DHCP enabled on Router
   * Unique MAC Address for Arduino
 
 Created: October 17, 2011 by Hans Scharler (http://www.nothans.com)
 
 Additional Credits:
 Example sketches from Arduino team, Ethernet by Adrian McEwen
 
*/
void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {
    //Serial.println("Starting post");         
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+THINGSPEAK_APIKEY+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
      Serial.println(tsData);
      
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
  
      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
}
