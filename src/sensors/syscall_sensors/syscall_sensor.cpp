/*
 *  File Name : syscall_sensor.cpp
 *  
 *  Creation Date : 05-09-2016
 *
 *  Last Modified : Thu 13 Apr 2017 01:46:04 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_readers/reader_factory.h"
#include "syscall_sensor.h"

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

// Deconstructor

Syscall_Sensor::~Syscall_Sensor(){

    stop_sensing();

    clear_observers();

    delete( reader );
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

    reader->update_filter();

    while ( is_sensing() )
    {
        Sensor_Data * tmp = sense_data();

        if ( tmp != NULL )
        {
            // CHECK: I have an idea.
            // I think that "data_point" keeps getting recreated on the stack.
            // It only goes out of scope when the while loop ends maybe?
            // So let's just... comment this out and then try to just pass
            // "*tmp" into "enqueue."  Who knows?

            /*
            Sensor_Data data_point( *tmp );
            data_queue.enqueue( data_point );
            */

            data_queue.enqueue( *tmp );
        }
        else
        {
            std::this_thread::yield();
        }

        delete ( tmp );
    }

    status = reader->stop_reading();
}

void Syscall_Sensor::notify_observers(){

    reader->update_filter();

    while ( is_sensing() )
    {

        // This method (try_dequeue) returns true if there is data in the
        // queue and we were able to assign it to the argument,
        // which is, in this case, data_point.        

        Sensor_Data data_point;

        if ( data_queue.try_dequeue( data_point ) )
        {
            // Iterate through our set of observers and
            // pass data_point to them.  It is passed
            // as an object and a copy constructor exists
            // for the Sensor_Data class.  Each observer
            // should have a copy of the data point.

            push_data( data_point );
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

// This method takes a Sensor_Data object and pushes it
// to all observers attached to this sensor.

void Syscall_Sensor::push_data( const Sensor_Data& data ){

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

uint_fast8_t Syscall_Sensor::start_sensing(){

    if ( !is_sensing() )
    {
        status = reader->start_reading();

        // We only want to start these threads if "start_reading" was successful,
        // which, if it was, will set status such that is_sensing() will now return true.

        if ( is_sensing() )
        {
            notify_thread = thread( &Syscall_Sensor::notify_observers, this );
            sense_thread = thread( &Syscall_Sensor::sense, this );
        }
    }

    return status;
}

uint_fast8_t Syscall_Sensor::stop_sensing(){

    if ( is_sensing() )
    {

        status &= ~SENSING_ON;
        sense_thread.join();
        notify_thread.join();

        // NOTE: This is if we want to empty the remaining
        // data on the queue by passing them to the observers.
        // We may wish not to do this.  We may wish to just
        // make the queue empty by deleting it.  We may wish
        // to make this controlled by a configuration option.
        // This is just how I decided for now.

        // UPDATE: This was very undesirable.
        // UPDATE 2: Stupid me didn't say *why* this was undesirable, and I think
        // I might actually need it.

        notify_thread = thread( &Syscall_Sensor::process_remaining_queue, this );

        // Wait until all remaining records are passed to
        // the observers...

        notify_thread.join();
    }
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
