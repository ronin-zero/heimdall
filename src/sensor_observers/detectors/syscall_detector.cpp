/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 27-12-2016
 *
 *  Last Modified : Mon 16 Jan 2017 11:16:04 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_detector.h"

Syscall_Detector::Syscall_Detector(){

    observing = false;
    processing = false;

    _call_formatter = new Syscall_Formatter();
}

Syscall_Detector::~Syscall_Detector(){

    delete ( call_formatter );
}

void Syscall_Detector::update(){

    // The default version is not needed at this time.
}

void Syscall_Detector::update( Sensor_Data data ){

    if ( observing )
    {
        data_queue.enqueue( data );
    }
}

void Syscall_Detector::set_observing( bool on ){

    if ( on )
    {
        start_observing();
    }
    else
    {
        stop_observing();
    }
}

void Syscall_Detector::set_processing( bool on ){

    if ( on )
    {
        start_processing();
    }
    else
    {
        stop_processing();
    }
}

bool Syscall_Detector::observing_status(){

    return observing;
}

bool Syscall_Detector::processing_status(){

    return processing;
}

void Syscall_Detector::start_observing(){

    observing = true;
}

void Syscall_Detector::start_processing(){

    if ( !processing )
    {
        processing = true;
        
        processing_thread = thread ( &Syscall_Detector::process, this );
    }
}

void Syscall_Detector::stop_observing(){

    observing = false;
}

void Syscall_Detector::stop_processing(){

    processing = false;
    // TODO: The rest of this method!
}

void Syscall_Detector::set_generator( NGram_Generator * generator ){

    _generator = generator;
}

void Syscall_Detector::set_trace_window( Trace_Window * window ){
    
    _window = window;
}

void Syscall_Detector::process(){

    // This will be set to false if one of the operations succeeded.
    // If none of them succeeded, we need to yield.

    bool process_success;

    while ( processing )
    {
        process_success = false; 

        process_success |= update_window();

        process_success |= generate_data();
        
        if ( !process_success )
        {
            std::this_thread::yield();
        }
    }
}

bool Syscall_Detector::update_window(){

    Sensor_Data sensor_data;

    if ( _window->trace_window_full() )
    {
        return false;
    }
    else if ( data_queue.try_dequeue( sensor_data ) )
    {
        record = Syscall_Record( sensor_data );

        uint_fast32_t syscall_num = record.get_syscall_num();

        return _window->add_data_point( syscall_num );
    }
    
    return false;
}

// TODO: This method has nothing.  Fix it.

bool Syscall_Detector::generate_data(){

    if ( _generator->has_next() )
    {
        return true;
    }

    return false;
}
