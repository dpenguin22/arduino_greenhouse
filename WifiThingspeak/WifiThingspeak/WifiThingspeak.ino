
/* WifiThingspeak -
  
    Purpose:
        Communication module for Greenhouse control.
        Based on DHTWServer: Version 1.0  5/3/2014 Mike Barela for Adafruit Industries

    Dependencies:
        ESP8266 library

    Assumptions/Limitations:
        Designed to run on an ESP8266 module, tested on an Adafruit Huzzah board
        
    Modification History:
        03/2016 - Initial version

*/


// Libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include <keys.h>

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
const int updateThingSpeakInterval = 1800 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

#define MAX_MESSAGE_SIZE 200

long lastConnectionTime = 0; 
boolean lastConnected = false;
int timeSinceConnection = 0;
int failedCounter = 0;
int messageSize;
boolean updateThing = false;

ESP8266WebServer server(80);
WiFiClient client;
int read_message(String* message, int bufsize);
void updateThingSpeak(String tsData);

String webString="";     // String to display
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
 
void handle_root() {
  server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
  delay(100);
}
 
void setup(void)
{
  
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  //Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  Serial.begin(9600);
  
  // Connect to WiFi network
  WiFi.begin(WiFiSSID, WiFiPSK);
  Serial.print("\n\r \n\rWorking to connect");

  // Set a static IP (optional)  
  IPAddress ip(192, 168, 1, 35);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
    
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Server");
  Serial.print("Connected to ");
  Serial.println(WiFiSSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", handle_root);
  
  //server.on("/temp", [](){  // if you add this subdirectory to your webserver call, you get text below :)
  //  gettemperature();       // read sensor
  //  webString="Temperature: "+String((int)temp_f)+" F";   // Arduino has a hard time with float to string
  //  server.send(200, "text/plain", webString);            // send to someones browser when asked
  //});
  server.on("/runFan", [] () { // if you add this subdirectory to your webserver call, you get the text below
      Serial.print("runFan\n");
      Serial.flush();
  });

  server.on("/openVent", [] () { // if you add this subdirectory to your webserver call, you get the text below
      Serial.print("openVent\n");
      Serial.flush();
  });
  
  server.on("/status", [] () { // if you add this subdirectory to your webserver call, you get the text below
      String incomingString = "";
      String respMsg = "";
      Serial.print("status\n");
      Serial.flush();
      delay(300);
      messageSize = read_message(&incomingString, MAX_MESSAGE_SIZE);
      if (incomingString.length() > 0) {
          respMsg = incomingString;   // assign to the response message                
      } else {
          respMsg = "The string was empty";
      }
      Serial.println(respMsg);
      webString="Test "+respMsg+" Message";
      server.send(200, "text/plain", webString);
  });
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void) {
    
    String incomingString = "";   // Received serial message
    String respMsg = "";          // HTTP Response message
  
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Attempting to re-establish connection to: ");
        Serial.println(WiFiSSID);
    }
    while (WiFi.status() != WL_CONNECTED)  {                   
        WiFi.begin(WiFiSSID, WiFiPSK);
        // Delays allow the ESP8266 to perform critical tasks
        // defined outside of the sketch. These tasks include
        // setting up, and maintaining, a WiFi connection.
        delay(500);
        Serial.print(".");
    }
    
    server.handleClient();
   
    // Disconnect from ThingSpeak
    if (!client.connected() && lastConnected)
    {
        //Serial.println("...disconnected");
        //Serial.println();
    
        client.stop();
    }
    client.stop();
    // Update ThingSpeak
    timeSinceConnection = millis() - lastConnectionTime;
    if (timeSinceConnection > 1.5*updateThingSpeakInterval) {
        // Something went wrong with the connection, reset
        lastConnectionTime = millis();
        updateThing = false;
    }
    if(!updateThing && !client.connected() && (timeSinceConnection > updateThingSpeakInterval))
    {
        updateThing = true;
        Serial.print("postData\n");
        //delay(500);
        Serial.flush();
    }
    if (updateThing && Serial.available() > 0) {
        messageSize = read_message(&incomingString, MAX_MESSAGE_SIZE);
      
        if (incomingString.length() > 0) {
            respMsg = incomingString;   // assign to the response message                
        } else {
            respMsg = "The string was empty";
        }
        Serial.print(F("Message size is: "));
        Serial.println(messageSize);
        Serial.print("The message: ");
        Serial.println(respMsg);
        updateThingSpeak(respMsg);
        updateThing = false;
    }
  
    // Check if Arduino Ethernet needs to be restarted
    // if (failedCounter > 3 ) {startEthernet();}
  
    lastConnected = client.connected();
  
} 
int read_message(String* message, int bufsize)
{
  String buffer = "";
  int count = 0;
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  // A three second watchdog timer is used in case the Serial buffer empties without the end of line char
  while (elapsedTime < 3000) {
      // Update timer
      elapsedTime = millis() - startTime;

      // Continue reading while new characters are available
      if (Serial.available()) {
          //char temp = Serial.peek();  // Check the next character (useful for debugging)
          //Serial.print(temp);
          char ch = Serial.read();    // read next character
          
          if (ch == '\n') {
              if (count > 1) {
                  buffer += '\0';              // end of line reached: null terminate string
                  *message = buffer;
                  return count;                // success: return length of string (zero if string is empty)
              } else {
                  // End of line was received after only 1 character, start over with the read 
                  buffer = "";
                  count = 0;
              }
          } else {
              buffer += ch; // Append character to buffer
              count += 1;
          }

          if (count > bufsize) {
              // Reached end of buffer, but have not seen the end-of-line yet.
              // Discard the rest of the line (safer than returning a partial line).

              *message = "The received message exceeded the buffer length";
              return -1; // error: return negative one to indicate the input was too long
          }
      }
  }
  // If this message is reached the watchdog timer expired
  *message = "No end of line character found";
  return -1;
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
      //Serial.println("Connecting to ThingSpeak...");
      //Serial.println();
      //Serial.println(tsData);
      
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

