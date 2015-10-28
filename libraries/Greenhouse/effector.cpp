/*
    Purpose:
        Effector class methods used to control analog or digital effectors
    
    Dependencies:
        Arduino library 

    Assumptions/Limitations:
        Assumes effectors are controlled through the analog or digital
        Arduino pins

    Modification History:
        10/2015 - Initial version

*/

#include "effector.h"
#include <arduino.h>

// Constructor. Initialize effector to the off state 
Effector::Effector(int effPin, bool inPolarity) {
    status = false;
    polarity = inPolarity;
    
    // Initialize to off
    if (inPolarity == 0) {
        // Effectors connected through a relay are active LOW so
        // initialize to HIGH
        digitalWrite(effPin, HIGH);
        analogWrite(effPin, HIGH);
    } else {
        // Otherwise initialize to LOW 
        digitalWrite(effPin, LOW);
        analogWrite(effPin, LOW);
    }
}

// Set the status of the device
void Effector::set_status(bool statusIn) {
    status = statusIn;

}

// Return the status of the device
bool Effector::get_status() {
    return status;

}

// Start the effector using a digital output
void Effector::start_digital(int effPin) {
    if (polarity == 0) {
        digitalWrite(effPin, LOW);
    } else {
        digitalWrite(effPin, HIGH);
    }
}

// Stop the effector using a digital output
void Effector::stop_digital(int effPin) {
    if (polarity == 0) {
        digitalWrite(effPin, HIGH);
    } else {
        digitalWrite(effPin, LOW);
    }
}

