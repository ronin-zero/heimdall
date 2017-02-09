/*
 *  File Name : training_counter.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 08 Feb 2017 07:03:52 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

#include "libsvm/svm.h"
#include "stopping_criterion.h"

class Training_Counter: public Stopping_Criterion{

    public:
        Training_Counter( uint_fast64_t num_samples_required );
        ~Training_Counter() {}

        bool start();
        bool can_stop_training();

    private:

        uint_fast64_t _sample_count;
        uint_fast64_t _num_samples_required;

};
