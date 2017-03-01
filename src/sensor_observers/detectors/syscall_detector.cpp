/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 12-27-2016
 *
 *  Last Modified : Wed 22 Feb 2017 11:36:04 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_detector.h"

Syscall_Detector::Syscall_Detector( Trace_Window * window, Data_Point_Generator * generator, Support_Vector_Generator * sv_generator, SVM_Module * svm_module ){

    // TODO:  Put that string in a constant or something.

    detection_log.open( "syscall_detector.log" );

    observing = false;
    processing = false;

    _window = window;
    _data_point_generator = generator;
    _sv_generator = sv_generator;
    _svm_module = svm_module;

    _call_formatter = new Syscall_Formatter();
}

Syscall_Detector::~Syscall_Detector(){

    stop_observing();
    stop_processing();

    detection_log << "Detection stopped at: " << current_time();
    detection_log.close();

    delete ( _call_formatter );
}

bool Syscall_Detector::train_from_saved_model( const std::string file_name ){

    return _svm_module->load_model( file_name );
}

// Ultimately, there should be a more elegant way of doing this.  As of now, 
// the syscall number needs to be the last field on a line in a trace.
// There should probably be a class that handles this with stuff like how many
// fields there are, what the separator is, and which field is the syscall number.

bool Syscall_Detector::train_from_trace( const std::string file_name, uint_fast8_t sep ){

    // TODO: Write this method to pull a system call trace from a trace file and use
    // it to train the svm_module.
   
    if ( processing )
    {
        return false;
    }
    
    std::ifstream trace_file( file_name );
    
    if ( trace_file.is_open() )
    {
        std::string line;

        uint_fast32_t line_count = 0;
        uint_fast32_t syscall_value;

        bool line_format_valid = true;

        while( std::getline( trace_file, line ) && line_format_valid )
        {
            size_t syscall_index = line.rfind(sep);

            std::string syscall_string;

            if ( syscall_index != std::string::npos )
            {
                syscall_string = line.substr( syscall_index + 1 );
            }
            else
            {
                syscall_string = line;
            }

            line_format_valid = ASCII_Operations::is_number( syscall_string );

            if ( line_format_valid )
            {
                // add data point.

                syscall_value = ASCII_Operations::to_uint( syscall_string );

                process_data_point( syscall_value );

                line_count++; 
            }
            else
            {
                // log failure, break loop.  give line number and offending line.
                // clean up training objects.
                // return false.
                
                detection_log << "ERROR - Malformed record at line #" << line_count << ": " << line << std::endl;
            }
        }

        if ( line_format_valid )
        {
            detection_log << "Finished reading trace_file. " << line_count << " lines read." << std::endl;
        }

        trace_file.close();

        return train_model();
        
    }
    else
    {
        detection_log << "ERROR: Could not open file " << file_name << std::endl;
    }
   
    return false;
}

bool Syscall_Detector::train_model(){

    if ( _svm_module->is_trained() )
    {
        detection_log << "ERROR: SVM_Module is already trained." << std::endl;

        return false;
    }

    char * message;

    time_t timer;

    if ( _svm_module->generate_model( message ) )
    {
        time(&timer);
        detection_log << "TRAINING SUCCEEDED at: ";
        detection_log << current_time() << std::endl;
        return true;
    }
    else
    {
        time(&timer);
        detection_log << "TRAINING FAILED at: ";
        detection_log << current_time() << std::endl;

        if ( message != NULL )
        {
            detection_log << "Error message: " << message << std::endl;
        }

        return false;
    }

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

    if ( processing )
    {
        processing = false;

        processing_thread.join();
    }
}

/*
bool Syscall_Detector::start_training(){

    if ( training )
    {
        return false;
    }
    else
    {
        training = true;
        return training;
    }
}

bool Syscall_Detector::stop_training(){

    if ( !training )
    {
        return false;
    }
    else
    {
        training = false;
        return training;
    }
}*/

void Syscall_Detector::set_generator( Data_Point_Generator * generator ){

    _data_point_generator = generator;
}

void Syscall_Detector::set_trace_window( Trace_Window * window ){
    
    _window = window;
}

char * Syscall_Detector::current_time(){

    time_t current_time;

    time( &current_time );

    return ctime(&current_time);
}

void Syscall_Detector::process(){

    // This will be set to false if one of the operations succeeded.
    // If none of them succeeded, we need to yield.

    while ( processing )
    {

        Sensor_Data data_point;
        
        if ( data_queue.try_dequeue( data_point ) )
        {
            Syscall_Record record( data_point );

            uint_fast32_t syscall_num = _call_formatter->format_syscall_num( record.get_syscall_num() );

            process_data_point( syscall_num );
        }
    }
}

void Syscall_Detector::process_data_point( uint_fast32_t data_point ){

    _window->add_data_point( _call_formatter->format_syscall_num( data_point ) );

    while ( _data_point_generator->has_next( _window ) && !_sv_generator->full() )
    {
        _sv_generator->add_data_point( _data_point_generator->generate_data_point( _window ) );
    }

    if ( _sv_generator->full() )
    {
        struct svm_node * node = _sv_generator->get_support_vector();

        process_data_vector( node );

        _sv_generator->reset();
    }

    if ( _data_point_generator->done( _window ) )
    {
        _window->reset_window();
        _data_point_generator->reset();
    }
}

void Syscall_Detector::process_data_vector( struct svm_node * node ){

    if ( !_svm_module->is_trained() )
    {
        _svm_module->add_training_vector( node, 0.0 );
    }
    else
    {
        double label = 0.0;

        if( _svm_module->predict( node, label ) )
        {
            detection_log << "CLASS: " << label << " predicted - " << current_time() << std::endl;
        }
        else
        {
            detection_log << "ERROR: Prediction failed - " << current_time() << std::endl;
        }
    }
}


/*
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
}*/

// This method checks to see if there is enough data in the trace window
// for the data point generator to generate a new data point.
// If the data point generator is capable of generating a new data point
// and the support vector generator is able to take another data point, 
// it generates a new point and adds it to the support vector generator
// and returns true.  Otherwise, it returns false.

/*
bool Syscall_Detector::generate_data(){

    if ( _data_point_generator->has_next( _window ) && !_sv_generator->full() )
    {
        int_fast64_t tmp_data_point = _data_point_generator->generate_data_point( _window );

        _sv_generator->add_data_point( tmp_data_point );

        return true;
    }

    return false;
}*/
