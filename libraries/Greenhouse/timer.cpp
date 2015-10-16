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
Timer::Timer() {
    value = 0;
    threshold = 0;
    increment = 0;
    frequency = 0;
}

// increment_timer will add the increment to the timer value
void Timer::increment_timer(unsigned long increment) {
    value = value + increment;

}

// Determine the threshold for triggering the timer
void Timer::set_threshold(float timerFreq) {
    // The input frequency is in Hz, the increment must be in ms
    threshold = (unsigned long) (1.0 / timerFreq) * 1000;

}

// reset_timer will reset the timer to zero
void Timer::reset_timer() {
    value = 0;
}

// get value will return the timer value
unsigned long Timer::get_value() {
    return value;
} 

// evaluate_timer will check the current value of the timer versus the threshold and 
// return whether it has been met. It will also reset the timer value
bool Timer::evaluate_timer() {
    if (value >= threshold) {
        reset_timer();
        return true;
    } else {
        return false;
    }
}

