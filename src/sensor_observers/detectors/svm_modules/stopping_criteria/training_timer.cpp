/*
 *  File Name : training_timer.cpp
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 08 Feb 2017 07:03:14 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "training_timer.h"

Training_Timer::Training_Timer( double training_time ){

    _training_time = training_time;
    _start_time = std::clock();
}

// This function allows users to start the timer some time after the object
// is created.  It returns a boolean value, but in this case, it is unlikely
// that the goal of this function will not be achieved, so it just returns true.
// Other Stopping_Criteria might have less trivial starting conditions.

bool Training_Timer::start(){

    _start_time = std::clock();
    return true;
}

bool Training_Timer::can_stop_training(){
    
    return ( _training_time <= ( std::clock() - _start_time ) / (double) CLOCKS_PER_SEC );
}
