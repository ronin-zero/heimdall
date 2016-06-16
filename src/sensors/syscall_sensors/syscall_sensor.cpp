/*
 *  File Name : syscall_sensor.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Thu 16 Jun 2016 04:25:56 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <iostream>

#include "sensors/syscall_sensors/syscall_readers/reader_factory.h"
#include "sensors/syscall_sensors/syscall_sensor.h"

Syscall_Sensor * Syscall_Sensor::ss_instance = NULL;

// Public Methods

Syscall_Sensor * Syscall_Sensor::get_instance( uint_fast8_t flags ){

    ss_instance = get_instance();

    ss_instance->configure( flags );

    return ss_instance;
}

Syscall_Sensor * Syscall_Sensor::get_instance(){

    if ( !ss_instance )
    {
        ss_instance = new Syscall_Sensor();
    }
    
    return ss_instance;
}

// Destructor.
Syscall_Sensor::~Syscall_Sensor(){

    stop_sensing();
}

uint_fast8_t Syscall_Sensor::configure( uint_fast8_t flags ){

    status = SENSING_OFF; 

    set_self_filter( flags & FILTER_SELF );
    set_enter( flags & SYS_ENTER );
    set_exit( flags & SYS_EXIT );
    
    // TODO--FIXED: This isn't good.  Change this so that
    // it calls "set_sensing" with the argument as flags
    // bit-wise anded with SENSING_ON.  (should be fine now)
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

    while ( is_sensing() )
    {
        Sensor_Data * data_point = sense_data();

        //std::cout << "I'M SENSIN, YO." << std::endl;

        if ( data_point != NULL )
        {
            data_queue.enqueue( *data_point );
        }
    }
}

void Syscall_Sensor::notify_observers(){


    while ( is_sensing() )
    {

        // This method (try_dequeue) returns true if there is data in the
        // queue and we were able to assign it to the argument,
        // which is, in this case, data_point.        

        //std::cout << "I'm notifying my observers, jabroni." << std::endl;

        Sensor_Data data_point;

        if ( data_queue.try_dequeue( data_point ) )
        {
            // Iterate through our set of observers and
            // pass data_queue to them.  It is passed
            // as an object and a copy constructor exists
            // for the Sensor_Data class.  Each observer
            // should have a copy of the data point.
            
            push_data( data_point );
        }
    } 
}

// This method takes a Sensor_Data object and pushes it
// to all observers attached to this sensor.

void Syscall_Sensor::push_data( Sensor_Data data ){

    for ( auto obs_it = observers.begin(); obs_it != observers.end(); ++obs_it )
    {
        (*obs_it)->update( data );
    }
}

// If sensing stops and there are still Sensor_Data objects in the queue, pass them to the
// observers until the queue is empty.
//
// NOTE: This functionality might not be desirable.  I have made the design decision that
// when a sensor is "sensing," it is both collecting data and reporting/pushing the data to its
// observers (notifying observers).  In the future, it may become clear that it would make more sense to
// separate the sensing and notifying processes so that data may be collected without necessarily being pushed
// and vice-versa
void Syscall_Sensor::process_remaining_queue(){

    Sensor_Data data_point;

    while ( data_queue.try_dequeue( data_point ) )
    {
        push_data( data_point );
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

// CHECK: Shawn recommends not using this method.
// It has redundant functionality.
// For now, I'm just going to comment it out.

/*
uint_fast8_t Syscall_Sensor::toggle_sensing(){

    // Just check if it's sensing and call the method
    // to set the sensing status, but with the
    // opposite value of the current sensing value.

    return set_sensing( !is_sensing() );
}
*/

uint_fast8_t Syscall_Sensor::start_sensing(){

    if ( !is_sensing() )
    {
        status = reader->start_reading();
    }

    sense_thread = thread( &Syscall_Sensor::sense, this );
    notify_thread = thread( &Syscall_Sensor::notify_observers, this );

    sense_thread.detach();
    notify_thread.detach();
    
    return status;
}

uint_fast8_t Syscall_Sensor::stop_sensing(){

    status = reader->stop_reading();

    // NOTE: This is if we want to empty the remaining
    // data on the queue by passing them to the observers.
    // We may wish not to do this.  We may wish to just
    // make the queue empty by deleting it.  We may wish
    // to make this controlled by a configuration option.
    // This is just how I decided for now.

    // UPDATE: This was very undesirable.

    // notify_thread = thread( &Syscall_Sensor::process_remaining_queue, this );

    // Wait until all remaining records are passed to
    // the observers...

    // notify_thread.join();

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

    std::cout << "CALLING THE SYSCALL_SENSOR CONSTRUCTOR WITH ARGS: " << int(flags) << std::endl;

    Reader_Factory factory ( flags );

    reader = factory.create_reader();

    status = reader->reading_status();
}
