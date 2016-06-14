/*
 *  File Name : record_factory.cpp
 *  
 *  Creation Date : 10-06-2016
 *
 *  Last Modified : Fri 10 Jun 2016 03:48:10 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensor_observers/data_records/record_factory.h"

uint_fast8_t Record_Factory::set_flags( uint_fast8_t new_flags ){

        flags = new_flags;
            
            return flags;
}

uint_fast8_t Record_Factory::get_flags(){
        
        return flags;
}
