/*
 *  File Name : data_point_generator.h
 *  
 *  Creation Date : 11-01-2017
 *
 *  Last Modified : Wed 22 Feb 2017 08:09:52 AM EST
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
        virtual bool done( Trace_Window * trace );

        virtual void reset();
};
