/*
 *  File Name : syscall_sensor.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Tue 17 May 2016 09:20:56 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"

Syscall_Sensor::Syscall_Sensor(){

    // Do later.

    status = SENSING_ON;

}

Syscall_Sensor * Syscall_Sensor::get_instance(){

    if ( !ss_instance ){
        ss_instance = new Syscall_Sensor();
    }

    return ss_instance;
}

uint_fast8_t Syscall_Sensor::set_exit ( bool on ){

    status = reader->set_exit( on );

    return status;
}

uint_fast8_t Syscall_Sensor::set_enter ( bool on ){

    status = reader->set_enter( on );
    
    return status;
}

uint_fast8_t Syscall_Sensor::configure( uint_fast8_t flags ){

    status = flags;

    return status;
}

uint_fast8_t Syscall_Sensor::sensing_status(){

    return status;
}

bool Sensor::is_sensing(){

    return true; // TODO: Fix later
}

void Sensor::sense(){

    //TODO: implement later.
}

Sensor_Data Sensor::sense_data(){

    Sensor_Data tmp("","","","");

    return tmp;
}

/*
uint_fast8_t Syscall_Sensor::set_self_filter( bool filter_self ){

    status = reader.set_filter_self( filter_self );

    return status;
}*/
