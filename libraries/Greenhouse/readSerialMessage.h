/*
    Purpose:
        Prototype for reading message over a serial connection

    Dependencies:
        Arduino library (to get the String class)
	Software serial library

    Assumptions/Limitations:
        None

    Modification History:
        11/2015 - Initial version
	12/2015 - Add software serial

*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>

#ifndef READSERIALMESSAGE_H
#define READSERIALMESSAGE_H

int read_serial_message(SoftwareSerial&, String *, int);

#endif
