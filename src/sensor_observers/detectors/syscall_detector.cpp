/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 27-12-2016
 *
 *  Last Modified : Tue 21 Feb 2017 08:05:37 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_detector.h"

Syscall_Detector::Syscall_Detector( Trace_Window * window, Data_Point_Generator * generator, Support_Vector_Generator * sv_generator, SVM_Module * svm_module ){

    observing = false;
    processing = false;

    _window = window;
    _data_point_generator = generator;
    _sv_generator = sv_generator;
    _svm_module = svm_module;

    _call_formatter = new Syscall_Formatter();
}

Syscall_Detector::~Syscall_Detector(){

    delete ( _call_formatter );
}

bool Syscall_Detector::train_from_saved_model( const std::string file_name ){

    return _svm_module->load_model( file_name );
}

bool Syscall_Detector::train_from_trace( const std::string file_name, uint_fast8_t sep ){

    // TODO: Write this method to pull a system call trace from a trace file and use
    // it to train the svm_module.

    return false;
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

void Syscall_Detector::set_generator( Data_Point_Generator * generator ){

    _data_point_generator = generator;
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

        // Add a new syscall to the trace winow if possible
        process_success |= update_window();

        // Add a new formatted data point to the sv generator if possible
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
        Syscall_Record record( sensor_data );

        uint_fast32_t syscall_num = _call_formatter->format_syscall_num( record.get_syscall_num() );

        return _window->add_data_point( syscall_num );
    }
    
    return false;
}

// This method checks to see if there is enough data in the trace window
// for the data point generator to generate a new data point.
// If the data point generator is capable of generating a new data point
// and the support vector generator is able to take another data point, 
// it generates a new point and adds it to the support vector generator
// and returns true.  Otherwise, it returns false.

bool Syscall_Detector::generate_data(){

    if ( _data_point_generator->has_next( _window ) && !_sv_generator->full() )
    {
        int_fast64_t tmp_data_point = _data_point_generator->generate_data_point( _window );

        _sv_generator->add_data_point( tmp_data_point );

        return true;
    }

    return false;
}
