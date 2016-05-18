/*
 *  File Name : syscall_sensor.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Wed 18 May 2016 10:08:40 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"
#include "sensors/syscall_sensors/syscall_readers/reader_factory.h"

Syscall_Sensor::Syscall_Sensor( uint_fast8_t flags ){

    Reader_Factory factory ( flags );

    reader = factory.create_reader();

    status = reader->reading_status();

}

Syscall_Sensor * Syscall_Sensor::get_instance( uint_fast8_t flags ){

    if ( !ss_instance ){
        ss_instance = new Syscall_Sensor( flags );
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

    status = 0x00;

    /*

    status |= reader->set_reading_on( flags | SENSING_ON );
    status |= reader->set_self_filter( flags | FILTER_SELF );
    status |= reader->set_enter( flags | SYS_ENTER );
    status |= reader->set_exit( flags | SYS_EXIT );
    */

    return status;
}

uint_fast8_t Syscall_Sensor::sensing_status(){

    return status;
}

bool Syscall_Sensor::is_sensing(){

    return ( status & SENSING_ON );
}

void Syscall_Sensor::sense(){

    //TODO: implement later.
}

Sensor_Data Syscall_Sensor::sense_data(){

    return reader->read_syscall();
}

uint_fast8_t Syscall_Sensor::set_self_filter( bool on ){

    status = reader->set_self_filter( on );
    return status;
}
