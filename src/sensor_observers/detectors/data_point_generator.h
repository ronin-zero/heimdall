/*
 *  File Name : data_point_generator.h
 *  
 *  Creation Date : 01-11-2017
 *
 *  Last Modified : Fri 30 Jun 2017 05:58:31 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

#include "trace_window.h"

class Data_Point_Generator{

    public: 

        virtual ~Data_Point_Generator() {}

        virtual int_fast64_t generate_data_point ( Trace_Window& trace, uint_fast32_t index = 0 )=0;
        
        virtual bool has_next( Trace_Window& trace, uint_fast32_t index = 0 )=0;
};
