/*
    Purpose:
        Function for reading a message over a serial connection

    Dependencies:
        Arduino library

    Assumptions/Limitations:
        Expects end-of-line character at the end of the message
	Intended to be called from within a "if (mySerial.available())" loop

    Modification History:
        11/2015 - Initial version
        12/2015 - changed function name
        03/2016 - Reads until the end of line character is found or the buffer size is exceeded

*/

// Libraries
#include "readSerialMessage.h"
#include <arduino.h>


int read_serial_message(SoftwareSerial& mySerial, String* message, int bufsize)
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
      if (mySerial.available()) {
          char ch = mySerial.read(); // read next character
          //Serial.print(ch); // echo it back: useful with the serial monitor (optional)
 
          if (ch == '\n') {
              buffer += '\0';              // end of line reached: null terminate string
              *message = buffer;
              return count;                // success: return length of string (zero if string is empty)
          }
          buffer += ch; // Append character to buffer
          count += 1;

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

