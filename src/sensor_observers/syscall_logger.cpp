/*
 *  File Name : syscall_logger.cpp
 *  
 *  Creation Date : 06-06-2016
 *
 *  Last Modified : Tue 21 Jun 2016 02:56:03 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensor_observers/syscall_logger.h"

Syscall_Logger::Syscall_Logger(){

    std::cout << "Syscall_Logger default constructor called." << std::endl;

    observing=false;
    processing=false;

    /*
    set_observing ( true );
    set_processing( true );
    */
}

Syscall_Logger::~Syscall_Logger(){

    std::cout << "Calling Syscall_Logger d'tor" << std::endl;
    stop_observing();
    stop_processing();

    //clear_streams();
    /*
    std::cout << "Processing remaining queue..." << std::endl;
    processing_thread = thread( &Syscall_Logger::process_remaining_queue, this );

    processing_thread.join();

    std::cout << "Finished." << std::endl;
    */
}

void Syscall_Logger::update(){

    // For this particular class,
    // we don't need update with no
    // arguments to do anything.
}

void Syscall_Logger::update( Sensor_Data data ){

    stream_mtx.lock();

    if ( observing )
    {
        data_queue.enqueue( data );
    }

    stream_mtx.unlock();
}

void Syscall_Logger::set_observing( bool on ){

    std::cout << "set_observing( " << on << " ) called." << std::endl;

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

    std::cout << "set_processing( " << on << " ) called." << std::endl;

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
        std::cout << "Starting processing." << std::endl;

        processing = true;

        processing_thread = thread ( &Syscall_Logger::process, this );
/*
        std::cout << "detatching processing thread." << std::endl;
        processing_thread.detach();
        std::cout << "Process thread detached." << std::endl;*/
    }
}

void Syscall_Logger::stop_observing(){

    observing = false;
}

void Syscall_Logger::stop_processing(){

    if ( processing )
    {
        processing = false;

        // processing_thread = thread ( &Syscall_Logger::process_remaining_queue, this );

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

    std::cout << "Processing Begins!" << std::endl;

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

    std::cout << "Done processing." << std::endl;
}

void Syscall_Logger::send_data( Syscall_Record record ){

    std::cout << "Sending data!" << std::endl;

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

    for ( auto stream_it = streams.begin(); stream_it != streams.end(); ++stream_it )
    {
        remove_stream( *stream_it );
        delete( *stream_it );
    }
}
