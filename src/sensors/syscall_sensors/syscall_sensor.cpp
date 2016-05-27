/*
 *  File Name : syscall_sensor.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Thu 26 May 2016 09:18:16 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensors/syscall_sensors/syscall_readers/reader_factory.h"

// Public Methods

Syscall_Sensor * Syscall_Sensor::get_instance( uint_fast8_t flags ){

    if ( !ss_instance )
    {
        ss_instance = new Syscall_Sensor( flags );
    }
    else
    {
        ss_instance->configure( flags );
    }

    return ss_instance;
}


uint_fast8_t Syscall_Sensor::configure( uint_fast8_t flags ){

    status = SENSING_OFF; 

    set_self_filter( flags & FILTER_SELF );
    set_enter( flags & SYS_ENTER );
    set_exit( flags & SYS_EXIT );
    
    // TODO: This isn't good.  Change this so that
    // it calls "set_sensing" with the argument as flags
    // bit-wise anded with SENSING_ON.
    //
    // status |= reader->set_reading( flags & SENSING_ON );

    status = set_sensing ( flags & SENSING_ON );    // CHECK: <--- I *think* this is better.

    return status;
}

Sensor_Data * Syscall_Sensor::sense_data(){

    return reader->read_syscall();
}

uint_fast8_t Syscall_Sensor::sensing_status(){

    return status;
}

bool Syscall_Sensor::is_sensing(){

    return ( status & SENSING_ON );
}

void Syscall_Sensor::sense(){

    // TODO: left off here when going to office hours.

    while ( is_sensing() )
    {
        Sensor_Data * data_point = sense_data();

        if ( data_point != NULL )
        {
            data_queue.enqueue( *data_point );
        }
    }
}

void Syscall_Sensor::notify_observers(){

    // TODO: Some base functionality if we don't have anything
    // to report to the observers.

    // UPDATE: I decided we don't need the version that takes
    // data as an argument.  It's really not needed.
    
    while ( is_sensing() )
    {
        Sensor_Data data_point;

        // This method returns true if there is data in the
        // queue and we were able to assign it to the argument,
        // which is, in this case, data_point.

        if ( data_queue.try_dequeue( data_point ) )
        {
            // Iterate through our set of observers and
            // pass data_queue to them.  It is passed
            // as an object and a copy constructor exists
            // for the Sensor_Data class.  Each observer
            // should have a copy of the data point.

            for ( auto obs_it = observers.begin(); obs_it != observers.end(); ++obs_it )
            {
                (*obs_it)->update( data_point );
            }
        }
    }
}

uint_fast8_t Syscall_Sensor::set_sensing( bool on ){

    if ( on )
    {
        return start_sensing();
    }
    else
    {
        return stop_sensing();
    }
}

uint_fast8_t Syscall_Sensor::toggle_sensing(){

    // Just check if it's sensing and call the method
    // to set the sensing status, but with the
    // opposite value of the current sensing value.

    return set_sensing( !is_sensing() );
}

uint_fast8_t Syscall_Sensor::start_sensing(){

    if ( !is_sensing() )
    {
        status = reader->start_reading();
    }
    return status;
}

uint_fast8_t Syscall_Sensor::stop_sensing(){

    status = reader->stop_reading();
    return status;
}

uint_fast8_t Syscall_Sensor::set_exit ( bool on ){

    status = reader->set_exit( on );

    return status;
}

uint_fast8_t Syscall_Sensor::set_enter ( bool on ){

    status = reader->set_enter( on );
    
    return status;
}

uint_fast8_t Syscall_Sensor::set_self_filter( bool on ){

    status = reader->set_self_filter( on );

    return status;
}




// Protected methods

Syscall_Sensor::Syscall_Sensor( uint_fast8_t flags ){

    Reader_Factory factory ( flags );

    reader = factory.create_reader();

    status = reader->reading_status();
}
