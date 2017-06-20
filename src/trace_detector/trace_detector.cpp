/*
 *  File Name : trace_detector.cpp
 *  
 *  Creation Date : 06-19-2017
 *
 *  Last Modified : Tue 20 Jun 2017 06:47:17 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "trace_detector.h"

Trace_Detector::Trace_Detector( Syscall_Formatter * formatter, size_t window_size, uint_fast32_t ngram_length ) :
    _one_class_svm(),
    _window( window_size ),
    _ngram_generator( ngram_length, formatter->table_size()),
    _sv_generator( window_size - ( ngram_length - 1 ) ){

        _call_formatter = formatter;

    }

void Trace_Detector::read_file_list( std::vector<std::string> files, uint_fast8_t sep ){

    for ( uint_fast32_t i; i < files.size(); i++ )
    {
        train_from_trace( files[i], sep );
    }
}

bool Trace_Detector::train_from_trace( std::string file_name, uint_fast8_t sep ){

    Trace_Reader reader( file_name, sep );

    // First, fill the trace window
    
    while ( reader.has_next() && !_window.trace_window_full() )
    {
        _window.add_data_point( reader.next_syscall() );
    }

    // Check if the loop broke because there weren't enough samples.
    // If that's why the loop stopped, return false.

    if ( !_window.trace_window_full() )
    {
        std::cerr << "ERROR: trace file " << file_name << " does not have enough records." << std::endl;
        return false;
    }

    // While there are more records, keep adding them to the trace window.
    
    while ( reader.has_next() )
    {
        _sv_generator.add_data_point( _ngram_generator.generate_data_point( _window ) );

        if ( _sv_generator.full() )
        {
            _one_class_svm.add_training_vector( _sv_generator.get_support_vector() );
            _sv_generator.reset();
        }

        _window.add_data_point( reader.next_syscall() );
    }

    // At this point, the file has no more records, but the window should be full.
    // Read the rest of the window and add the ngrams to the support vector generator.
    // There will most likely be overlap between the last two trace windows.

    for ( uint_fast32_t i = 0; !_sv_generator.full() && _ngram_generator.has_next( _window, i ); i++ )
    {
        _sv_generator.add_data_point( _ngram_generator.generate_data_point( _window, i ) );
    }

    _one_class_svm.add_training_vector( _sv_generator.get_support_vector() );

    _sv_generator.reset();

    std::cout << "Finished reading file " << file_name << std::endl;

    return true;
}

bool Trace_Detector::train_model(){
    
    if ( _one_class_svm.is_trained() )
    {
        std::cerr << "ERROR: One_Class_SVM already trained." << std::endl;
        return false;
    }

    char * message = NULL;

    if ( _one_class_svm.generate_model( message ) )
    {
        std::cout << "Training succeeded." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "ERROR: Training failed." << std::endl;

        if ( message != NULL )
        {
            std::cerr << "Error message: " << message << std::endl;
        }
    }

    return false;
}
