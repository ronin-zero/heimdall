/*
 *  File Name : training_counter.cpp
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 08 Feb 2017 07:04:30 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "training_counter.h"

// This takes as an argument a nonnegative number that will represent
// how many samples must be observed before training can end.

Training_Counter::Training_Counter( uint_fast64_t num_samples_required ){

    _num_samples_required = num_samples_required;
    _sample_count = 0;
}

// This probably isn't necessary, but it should be called at the "start"
// of training.  It's more important for Stopping_Criterion subclasses
// that base the decision of whether to stop training on time elapsed.
// The "clock" starts in the constructor just as _sample_count is set
// to 0 in this class's constructor, but in the classes using time,
// it is possible that training won't begin until some time after
// the constructor is called.
//
// Since start is a pure virtual function, however, it requires a definition.
//
// Since we are unlikely to fail setting the value of _sample_count to 0,
// it should just return true (other Stopping_Criteria might have something
// less trivial they need to check).

bool Training_Counter::start(){

   _sample_count = 0;
   return true;
}

// This function will be called every time a training example is added to 
// the svm_module holding the pointer to it.  So, we increment
// the count by one and check to see if the number of required samples
// has been reached in a single line.

bool Training_Counter::can_stop_training(){

    return ++_sample_count >= _num_samples_required;
}
