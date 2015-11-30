/*
    Purpose:
        Prototype for reading message over a serial connection

    Dependencies:
        Arduino library (to get the String class)

    Assumptions/Limitations:
        None

    Modification History:
        11/2015 - Initial version

*/

#include <Arduino.h>

#ifndef READSERIALMESSAGE_H
#define READSERIALMESSAGE_H

int read_message(String *, int);

#endif
