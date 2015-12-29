/*
    Purpose:
        Function for processing received requests

    Dependencies:
        Adafruit CC3000 library
        readSerialMessage

    Assumptions/Limitations:

    Modification History:
        11/2015 - Initial version

*/

// Libraries
#include "setupWifi.h"
#include "readSerialMessage.h"


void process_wifi()
{

    String incomingString = "";   // Received serial message
    String respMsg = "";          // HTTP Response message
    int messageSize;

    // Check if a client has connected
    Adafruit_CC3000_ClientRef client = server.available();
    if (!client) {
      return;
    }
    
    // Wait until the client sends some data
    Serial.println("new client");
    while(!client.available()){
      delay(1);
    }
 
    // Read the first line of the request
    String req = client.readStringUntil('\r');
    
    // Echo the request
    Serial.println(req);
    client.flush();

    if (req.indexOf("/test/0") != -1) {
        respMsg = "Got signal 0";
        Serial.println("Got signal 0");
    } else if (req.indexOf("/test/1") != -1) {
        respMsg = "Got signal 1";
        Serial.println("Got signal 1");
    } else if (req.indexOf("/test/read") != -1) {
        // send data only when you receive data:
        if (Serial.available() > 0) {
//            messageSize = read_message(&incomingString, MAX_MESSAGE_SIZE);
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
    
        respMsg = "Invalid reqest";
        Serial.println("invalid request");
        client.stop();
        return;
    }
  
   // Prepare the response
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
   if (respMsg.length() > 0) {
       s += respMsg;
   } else {
       s += "OK";
   } 
   s += "\n";

   // Send the response to the client
   client.print(s);
   delay(1);
   client.stop();
   Serial.println("Client disconnected");

}


