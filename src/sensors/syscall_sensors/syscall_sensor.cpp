/*
 *  File Name : syscall_sensor.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Mon 09 May 2016 03:35:28 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_sensor.h"

Syscall_Sensor::Syscall_Sensor(){

    // Do later.

}

static Syscall_Sensor & Syscall_Sensor::get_instance(){

    if ( !ss_instance ){
        ss_instance = new Syscall_Sensor();
    }
}

fast_uint8_t Syscall_Sensor::set_exit ( bool on ){

    status = reader.set_exit( on );
}

fast_uint8_t Syscall_Sensor::set_enter ( bool on ){

    status = reader.set_enter( on );
}

fast_uint8_t Syscall_Sensor::set_self_filter( bool filter_self ){

    status = reader.set_filter_self( filter_self );
}
