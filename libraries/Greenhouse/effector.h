/*
    Purpose:
        Definition of the effector class
    
    Dependencies:
        None

    Assumptions/Limitations:
        None

    Modification History:
        10/2015 - Initial version

*/

#ifndef EFFECTOR_H
#define EFFECTOR_H


class Effector {

    public:
        // Variables

        // Methods
        Effector(int, bool);          // Constructor
        void set_status(bool);        // Set the on/off status of the effector
        bool get_status();            // Get the on/off status of the effector
        void start_digital(int);      // Start the effector using a digital output
        void stop_digital(int);       // Stop the effector using a digital output

    private:
        // Variables
        bool status;                  // On/off value of the effector
        bool polarity;                // Polariy of the effecor, 0 means it is active LOW
};

#endif
