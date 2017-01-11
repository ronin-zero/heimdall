/*
 *  File Name : data_point_generator.h
 *  
 *  Creation Date : 11-01-2017
 *
 *  Last Modified : Wed 11 Jan 2017 02:23:28 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

#include "trace_window.h"

class Data_Point_Generator{

    public: 

        Data_Point_Generator();
        ~Data_Point_Generator() {}

        virtual int_fast64_t generate_data_point ( Trace_Window * trace )=0;
        
        virtual bool has_next( Trace_Window * trace )=0;
        virtual bool done();

};
