/*
    Purpose:
        Function for reading a message over a serial connection

    Dependencies:
        Arduino library

    Assumptions/Limitations:
        Expects end-of-line character at the end of the message

    Modification History:
        11/2015 - Initial version
        12/2015 - changed function name

*/

// Libraries
#include "readSerialMessage.h"
#include <arduino.h>


int read_serial_message(SoftwareSerial& mySerial, String* message, int bufsize)
{
  String buffer = "";
  for (int index = 0; index < bufsize; index++) {

    // Wait until characters are available
    while (mySerial.available() == 0) {
    }
    
    char ch = mySerial.read(); // read next character
    //Serial.print(ch); // echo it back: useful with the serial monitor (optional)
    //Serial.print(buffer);
 
    if (ch == '\n') {
      buffer += '\0';          // end of line reached: null terminate string
      *message = buffer;
      mySerial.println(*message);  // print full message before exiting
      return index;            // success: return length of string (zero if string is empty)
    }
    buffer += ch; // Append character to buffer
  }

  // Reached end of buffer, but have not seen the end-of-line yet.
  // Discard the rest of the line (safer than returning a partial line).

  *message = "The received message exceeded the buffer length";
  mySerial.println(*message);
  return -1; // error: return negative one to indicate the input was too long
}

