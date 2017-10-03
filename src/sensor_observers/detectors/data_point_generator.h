/*
 *  File Name : data_point_generator.h
 *  
 *  Creation Date : 01-11-2017
 *
 *  Last Modified : Tue 03 Oct 2017 12:46:24 PM EDT
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
        virtual uint_fast32_t num_data_points( Trace_Window trace )=0;
        
        virtual bool has_next( Trace_Window& trace, uint_fast32_t index = 0 )=0;
};
