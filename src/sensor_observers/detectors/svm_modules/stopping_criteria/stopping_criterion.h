/*
 *  File Name : stopping_criterion.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 08 Feb 2017 07:03:36 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

#include "libsvm/svm.h"

class Stopping_Criterion{

    public:
        virtual bool start() = 0;
        virtual bool can_stop_training() = 0;
};
