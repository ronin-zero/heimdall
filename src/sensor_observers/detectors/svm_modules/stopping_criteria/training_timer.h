/*
 *  File Name : training_timer.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 08 Feb 2017 07:04:08 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <ctime>

#include "stopping_criterion.h"
#include "libsvm/svm.h"

class Training_Timer: public Stopping_Criterion{

    public:

        Training_Timer( double training_time );
        ~Training_Timer() {}

        bool start();
        bool can_stop_training();

    private:

        std::clock_t _start_time;
        double _training_time;

};
