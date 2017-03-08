/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 12-27-2016
 *
 *  Last Modified : Tue 07 Mar 2017 05:32:38 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_detector.h"

Syscall_Detector::Syscall_Detector( size_t window_size, uint_fast32_t ngram_length ) : _call_formatter(), _window( window_size ), _ngram_generator( ngram_length, TABLE_SIZE ),
                                                                                        _sv_generator( window_size - ( ngram_length - 1 )), _svm_module(){

    
    // TODO:  Put that string in a constant or something.

    detection_log.open( "syscall_detector.log" );

    observing = false;
    processing = false;

}

Syscall_Detector::~Syscall_Detector(){

    detection_log << "Detection stopped at: " << current_time();
    detection_log.close();
}

bool Syscall_Detector::train_from_saved_model( const std::string file_name ){

    return _svm_module.load_model( file_name );
}

// Ultimately, there should be a more elegant way of doing this.  As of now, 
// the syscall number needs to be the last field on a line in a trace.
// There should probably be a class that handles this with stuff like how many
// fields there are, what the separator is, and which field is the syscall number.

bool Syscall_Detector::train_from_trace( const std::string file_name, uint_fast8_t sep ){

    detection_log << "Training from trace beginning at: " << current_time() << std::endl;

    Trace_Reader reader( file_name, sep );

    // First, fill the trace window

    while ( reader.has_next() && !_window.trace_window_full() )
    {
        _window.add_data_point( reader.next_syscall() );
    }

    // Check if the loop broke because there weren't enough samples in the tracefile,
    // in which case, return false.  There is (at most) one full trace, which isn't enough.

    if ( !_window.trace_window_full() )
    {
        detection_log << "ERROR:  Training failed at " << current_time() << std::endl;

        return false;
    }

    // While the file has more records, keep adding them to the trace window.  If the window
    // is full when a record is added, it will dequeue the first elemement.  This way, the
    // window will always be full.  Until the file is out of records, the windows will not
    // overlap.

    while ( reader.has_next() )
    {
        _sv_generator.add_data_point( _ngram_generator.generate_data_point( _window ) );

        if ( _sv_generator.full() )
        {
            _svm_module.add_training_vector( _sv_generator.get_support_vector() );
            _sv_generator.reset();
        }

        _window.add_data_point( reader.next_syscall() );
    }

    // At this point, the file has no more records, but the window should be full.
    // Read the rest of the window and add the ngrams to the support vector generator.
    // There will most likely be overlap between the last two trace windows.

    for ( uint_fast32_t i = 0;  !_sv_generator.full() && _ngram_generator.has_next( _window, i ); i++ )
    {
        _sv_generator.add_data_point( _ngram_generator.generate_data_point( _window, i ) );
    }

    _svm_module.add_training_vector( _sv_generator.get_support_vector() );

    _sv_generator.reset();

    detection_log << "Training from trace complete at " << current_time() << std::endl;

    return train_model();
}

bool Syscall_Detector::train_model(){

    if ( _svm_module.is_trained() )
    {
        detection_log << "ERROR: SVM_Module is already trained." << std::endl;

        return false;
    }

    char * message;

    time_t timer;

    if ( _svm_module.generate_model( message ) )
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
}

void Syscall_Detector::set_generator( Data_Point_Generator * generator ){

    _ngram_generator = generator;
}

void Syscall_Detector::set_trace_window( Trace_Window * window ){
    
    _window = window;
}
*/

bool Syscall_Detector::save_model( const std::string file_name ){

    int_fast32_t result = _svm_module.save_model( file_name );

    if ( result != 0 )
    {
        std::cerr << "ERROR:  Model could not be saved to file " << file_name << std::endl;
    }

    return result == 0;
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

            send_data( record );
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

void Syscall_Detector::send_data( Syscall_Record& record ){

    // TODO: Later, this should determine whether we are training or testing, but for now,
    // just assume it's testing.

    process_data_point( record.get_syscall_num() );
}

void Syscall_Detector::process_data_point( uint_fast32_t data_point ){

    _window.add_data_point( _call_formatter.format_syscall_num( data_point ) );

    _sv_generator.add_data_point( _ngram_generator.generate_data_point( _window ) );

    if ( _sv_generator.full() )
    {
        struct svm_node * node = _sv_generator.get_support_vector();

        process_data_vector ( node );

        _sv_generator.reset();
    }

/*    while ( _data_point_generator.has_next( _window ) && !_sv_generator.full() )
    {
        _sv_generator.add_data_point( _data_point_generator.generate_data_point( _window ) );
    }

    if ( _sv_generator.full() )
    {
        struct svm_node * node = _sv_generator.get_support_vector();

        process_data_vector( node );

        _sv_generator.reset();
    }

    if ( _data_point_generator.done( _window ) )
    {
        _window.reset_window();
        _data_point_generator.reset();
    }*/
}

void Syscall_Detector::process_data_vector( struct svm_node * node ){

    if ( !_svm_module.is_trained() )
    {
        _svm_module.add_training_vector( node, 0.0 );
    }
    else
    {
        double label = 0.0;

        if( _svm_module.predict( node, label ) )
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
