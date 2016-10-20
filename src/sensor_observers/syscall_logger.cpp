/*
 *  File Name : syscall_logger.cpp
 *  
 *  Creation Date : 06-06-2016
 *
 *  Last Modified : Wed 19 Oct 2016 08:19:25 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_logger.h"

// Debug stuff
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#define gettid() syscall(SYS_gettid)

Syscall_Logger::Syscall_Logger(){

    observing=false;
    processing=false;
}

// Deconstructor

Syscall_Logger::~Syscall_Logger(){

    stop_processing();
    stop_observing();

    clear_streams();
}

void Syscall_Logger::update(){

    // For this particular class,
    // we don't need update with no
    // arguments to do anything.
}

void Syscall_Logger::update( Sensor_Data data ){

    if ( observing )
    {
        data_queue.enqueue( data );
    }
}

void Syscall_Logger::set_observing( bool on ){

    if ( on )
    {
        start_observing();
    }
    else
    {
        stop_observing();
    }
}

void Syscall_Logger::set_processing( bool on ){

    if ( on )
    {
        start_processing();
    }
    else
    {
        stop_processing();
    }
}

bool Syscall_Logger::observing_status(){

    return observing;
}

bool Syscall_Logger::processing_status(){

    return processing;
}

void Syscall_Logger::start_observing(){

    observing = true;
}

void Syscall_Logger::start_processing(){

    if ( !processing )
    {
        processing = true;

        processing_thread = thread ( &Syscall_Logger::process, this );
    }
}

void Syscall_Logger::stop_observing(){

    observing = false;
}

void Syscall_Logger::stop_processing(){

    if ( processing )
    {
        processing = false;

        processing_thread.join();
    }
}

void Syscall_Logger::add_stream( Data_Stream * stream ){

    streams.insert( stream );
}

void Syscall_Logger::remove_stream( Data_Stream * stream ){

    streams.erase( stream );
}

void Syscall_Logger::process(){

    std::cout << "(THREAD " << gettid() << ") Syscall_Logger beginning \"process()\" thread..." << std::endl;


    while ( processing )
    {
        Sensor_Data data_point;

        // try_dequeue attempts to remove the next
        // item on the queue and assigns it to the
        // reference given as an argument (in this
        // case, data_point).
        //
        // Note that this is similar to
        // the notify_observers method in
        // Syscall_Sensor.cpp.

        if ( data_queue.try_dequeue( data_point ) )
        {
            Syscall_Record syscall_record( data_point );

            send_data( syscall_record );
        }
    }

    std::cout << "(THREAD " << gettid() << ") Syscall_Logger process() thread ending." << std::endl;
}

// CHECK: I'm a bit worried about passing a reference, here. So far, there is only a single observer,
// so I don't know if deep/shallow copy will be an issue.
void Syscall_Logger::send_data( Syscall_Record record ){

    for ( auto stream_it = streams.begin(); stream_it != streams.end(); ++stream_it ){

        (*stream_it)->process_data( &record );
    }
}

void Syscall_Logger::process_remaining_queue(){

    // NOTE: As with Syscall_Sensor, this behavior
    // may not be desirable.  This continues to send
    // data to data_streams until it's empty.
    // This will happen if there are still Sensor_Data
    // objects on the queue when processing ends.
    // This is a somewhat arbitrary decision,
    // and in the future, we may wish to disable this.

    Sensor_Data data_point;

    while ( data_queue.try_dequeue( data_point ) )
    {
        Syscall_Record syscall_record( data_point );

        send_data( syscall_record );
    }
}

void Syscall_Logger::clear_streams(){

    streams.clear();
}
