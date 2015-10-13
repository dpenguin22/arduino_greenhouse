/*
    Purpose:
        Definition of the timer class
    
    Dependencies:
        None

    Assumptions/Limitations:
        None

    Modification History:
        10/2015 - Initial version

*/

#ifndef TIMER_H
#define TIMER_H


class Timer {

    public:
        // Variables
        float frequency;      // Frequency in Hz that the timer will run at
        unsigned long increment; // Time to increment the timer (s) Will be 
                                 // how fast the scheduler is being called

        // Methods
        Timer();  // Constructor
        void determine_increment(float); // Determine the increment based on the calling frequency
        void reset_timer();   // Reset the value of the timer
        void increment_timer(unsigned long, unsigned long); // Increment the value of the timer
        unsigned long get_value(); // Return the value of the timer
        bool evaluate_timer(unsigned long, unsigned long); // Evaluate whether it is time to execute the 
                                                           // job based on the timer

    private:
        // Variables
        unsigned long value;  // Current value of the timer (s) 
}

#endif
