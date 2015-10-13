/*
    Purpose:
        Timer class methods used to increment a timer and determine when it
        has expired.
    
    Dependencies:
        None

    Assumptions/Limitations:
        Designed for a recurring timer that triggers at a data driven frequency

    Modification History:
        10/2015 - Initial version

*/

#include "timer.h"

// Constructor. Initialize to zero
void Timer::Timer() {
    value = 0;
    interval = 0;
    frequency = 0;
}

// increment_timer will add the increment to the timer value
void increment_timer(unsigned long increment, unsigned long value) {
    value = value + increment;

}

// Determine the increment for the timer based on the 
void determine_increment(float callFreq) {
    increment = frequency / callFreq;

}

// reset_timer will reset the timer to zero
void reset_timer() {
    value = 0;
}

// get value will return the timer value
unsigned long get_value {
    return value;
} 

// evaluate_timer will check the current value of the timer versus the threshold and 
// return whether it has been met. It will also reset the timer value
bool evaluate_timer(unsigned long threshold, unsigned long value) {
    if (value >= threshold) {
        reset_timer();
        return true;
    } else {
        return false;
}

