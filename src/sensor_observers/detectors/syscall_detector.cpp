/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 12-27-2016
 *
 *  Last Modified : Mon 27 Nov 2017 09:26:09 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_detector.h"

Syscall_Detector::Syscall_Detector( size_t window_size, uint_fast32_t ngram_length, std::string detection_log_file_name, uint_fast8_t arch) : 
    _window( window_size ), 
    _sv_generator( window_size - ( ngram_length - 1 )), 
    _svm_module() 
{


    // TODO:  Put that string in a constant or something.

    detection_log.open( detection_log_file_name );

    observing = false;
    processing = false;

    _arch = arch;

    if ( _arch & ARCH_ARM )
    {
        _call_formatter = new ARM_Syscall_Formatter();
    }
    else if ( _arch & ARCH_MIPS )
    {
        switch ( _arch & OPTS_MASK ) {
            case OABI_32:
                _call_formatter = new MIPS_o32_Syscall_Formatter();
                break;
            case NABI_32:
                _call_formatter = new MIPS_n32_Syscall_Formatter();
                break;
            case NABI_64:
                _call_formatter = new MIPS_n64_Syscall_Formatter();
                break;
            default:
                _call_formatter = new MIPS_Syscall_Formatter();
                break;
        }
    }
    else
    {
        _call_formatter = new Syscall_Formatter();
    }

    _ngram_generator = new NGram_Generator( ngram_length, _call_formatter->table_size() );
}

Syscall_Detector::~Syscall_Detector(){

    delete _call_formatter;
    delete _ngram_generator;
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

bool Syscall_Detector::train_from_trace( const std::string file_name, double class_label, uint_fast8_t sep ){

    // Don't try to train if it's already trained.

    if ( _svm_module.is_trained() ) {
        return false;
    }

    bool prev_processing_status = processing_status();

    // We don't want the sensor to be training in real-time while we train offline.

    stop_processing();

    /*  create Trace_Reader object
     *  call get_trace_vectors
     *  for each trace vector (struct svm_node *), call process_data_vector
     */



    set_processing( prev_processing_status );

    /*
       bool training_result_status = false;

       if ( ! _svm_module.is_trained() ) 
       {
       bool prev_processing_status = processing_status();

       stop_processing();

       detection_log << "TRAINING TRACE FILE: " << file_name << std::endl;

       Trace_Reader reader( file_name, sep );

       _window.clear();
       _sv_generator.reset();

       uint_fast32_t syscall_records   = 0;
       uint_fast32_t ngrams            = 0;
       uint_fast32_t training_vectors  = 0;

       training_result_status = true;

       while ( reader.has_next() )
       {
       int_fast32_t curr_syscall = reader.next_syscall();

       if ( curr_syscall >= 0 )
       {

       _window.add_data_point( _call_formatter->format_syscall_num( curr_syscall ) );
       syscall_records++;

       if ( _ngram_generator->has_next( _window ) )
       {
       int_fast64_t ngram = _ngram_generator->generate_data_point( _window );

       if ( ngram >= 0 )
       {
       _sv_generator.add_data_point( ngram );
       ngrams++;
       }
       }

       if ( _sv_generator.full() )
       {
       struct svm_node * current_node = _sv_generator.get_support_vector();

       if ( current_node != NULL )
       {
       _svm_module.add_training_vector( current_node, 0.0 );
       training_vectors++;
       }

       _sv_generator.reset();
       }
       }
       }

    // At this point, the file has no more records, but the window should be full.
    // Read the rest of the window and add the ngrams to the support vector generator.
    // There will most likely be overlap between the last two trace windows.

    if ( !_window.trace_window_full() )
    {
    detection_log << "ERROR: Not enough records to make a data point or file does not exist." << std::endl;
    training_result_status = false;
    }
    else
    {
    uint_fast32_t window_index = 0;

    while ( !_sv_generator.full() && _ngram_generator->has_next( _window, window_index ) )
    {
        _sv_generator.add_data_point( _ngram_generator->generate_data_point( _window, window_index ) );
        ngrams++;
        window_index++;
    }

    if ( _sv_generator.full() )
    {
        struct svm_node * current_node = _sv_generator.get_support_vector();

        if ( current_node != NULL )
        {
            _svm_module.add_training_vector( current_node, 0.0 );
            training_vectors++;
        }

        _sv_generator.reset();
    }

    // Check if there is even one syscall in the window that hasn't been included.
    // If there is, make a new vector out of the remaining calls (even if there 
    // is a slight overlap between the last two windows/vectors)

    if ( _ngram_generator->has_next( _window, window_index ) )
    {
        while ( _ngram_generator->has_next( _window, 0 ) )
        {
            _sv_generator.add_data_point( _ngram_generator->generate_data_point( _window, 0 ) );
            ngrams++;
            _window.pop_front();
        }

        struct svm_node * current_node = _sv_generator.get_support_vector();

        if ( current_node != NULL )
        {
            _svm_module.add_training_vector( current_node, 0.0 );
            training_vectors++;
        }

        _sv_generator.reset();

    }

    training_result_status = true;
}
// Report the stats of the file

detection_log << "  Syscall_Records:    " << syscall_records << std::endl;
detection_log << "  Ngrams:             " << ngrams << std::endl;
detection_log << "  Training_Vectors:   " << training_vectors << std::endl;

detection_log << "TRAINING FILE " << file_name << " finished." << std::endl << std::endl;

_window.clear();
_sv_generator.reset();

set_processing( prev_processing_status );
}
else
{
    detection_log << "ERROR: Training -- Module already trained!" << std::endl;
}

return training_result_status;*/
}

double Syscall_Detector::test_trace_file( const std::string file_name, uint_fast8_t sep ){

    double percent_anomaly = 0.0;

    uint_fast32_t call_trace_vectors   = 0;
    uint_fast32_t anomalies     = 0;

    std::cout << "TESTING FILE: " << file_name << std::endl;

    Trace_Reader reader( file_name, sep );

    bool prev_processing_status = processing_status();

    stop_processing();


    if ( _svm_module.is_trained() ) 
    {

        _window.clear();
        _sv_generator.reset();

        while ( reader.has_next() )
        {
            _window.add_data_point( _call_formatter->format_syscall_num( reader.next_syscall() ) );

            if ( _ngram_generator->has_next( _window ) )
            {
                _sv_generator.add_data_point( _ngram_generator->generate_data_point( _window ) ); 
            }

            if ( _sv_generator.full() )
            {
                struct svm_node * test_vector = _sv_generator.get_support_vector();

                double label = 0.0;

                if ( test_vector != NULL )
                {

                    if ( _svm_module.predict( test_vector, label ) )
                    {
                        if ( label < 0.0 )
                        {
                            anomalies++;
                        }
                    }
                }

                detection_log << call_trace_vectors << "    " << label << std::endl;
                call_trace_vectors++;

                _sv_generator.reset();
            }
        }

        if ( _window.trace_window_full() )
        {
            // Finish the last test vector (even with overlap)

            uint_fast32_t window_index = 0;
            while ( _ngram_generator->has_next( _window, window_index ) && !_sv_generator.full() )
            {
                _sv_generator.add_data_point( _ngram_generator->generate_data_point( _window, window_index ) ); 
                window_index++;
            }

            if ( _sv_generator.full() )
            {
                struct svm_node * test_vector = _sv_generator.get_support_vector();

                double label = 0.0;

                if ( test_vector != NULL )
                {
                    if ( _svm_module.predict( test_vector, label ) )
                    {
                        if ( label < 0.0 )
                        {
                            anomalies++;
                        }
                    }
                }
                detection_log << call_trace_vectors << "    " << label << std::endl;
                call_trace_vectors++;
            }

            _sv_generator.reset();

            // Check that there are data points in the window that weren't
            // included in a model yet.

            if ( _ngram_generator->has_next( _window, window_index ) )
            {
                // Empty the current window into the generator, get a vector, and test it.

                while ( _ngram_generator->has_next( _window, 0 ) )
                {
                    _sv_generator.add_data_point( _ngram_generator->generate_data_point( _window, 0 ) );
                    _window.pop_front();
                }

                struct svm_node * test_vector = _sv_generator.get_support_vector();
                double label = 0.0;

                if ( test_vector != NULL )
                {
                    if ( _svm_module.predict( test_vector, label ) )
                    {
                        if ( label < 0.0 )
                        {
                            anomalies++;
                        }
                    }
                }

                detection_log << call_trace_vectors << "    " << label << std::endl;
                call_trace_vectors++;
            }
        }

        if ( call_trace_vectors > 0 )
        {
            // TODO:  This is a paranoid level of double casting.

            percent_anomaly = (double) ( (double) anomalies / (double) call_trace_vectors );
        }

        _window.clear();
        _sv_generator.reset();
    }
    else
    {
        percent_anomaly = -1.0;
    }

    detection_log << "RESULTS for file: " << file_name << std::endl;
    detection_log << "TEST_VECTORS: " << call_trace_vectors << std::endl;
    detection_log << "ANOMALIES:    " << anomalies << std::endl;
    detection_log << "PERCENT:      " << percent_anomaly << std::endl << std::endl;

    set_processing( prev_processing_status );

    return percent_anomaly;
}

bool Syscall_Detector::train_model(){

    if ( _svm_module.is_trained() )
    {
        detection_log << "ERROR: SVM_Module is already trained." << std::endl;

        return false;
    }

    char * message = NULL;

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
    process_data_point( data_point, _window, _sv_generator );
}

void Syscall_Detector::process_data_point( uint_fast32_t data_point, Trace_Window & window, Support_Vector_Generator & sv_generator ){

    uint_fast32_t syscall_num = _call_formatter->format_syscall_num( data_point );

    window.add_data_point( syscall_num );

    if ( window.trace_window_full() )
    {
        sv_generator.add_data_point( _ngram_generator->generate_data_point( window ) );
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

// TODO: The same 3 lines of code appear 3 times within this method.  This should be streamlined.
// As of writing this (10/3/2017), this comment appears on lines 643-645.
// The offending blocks of code appear at 661-663, 681-683, and kind of at 674-675.

std::vector<struct svm_node *> Syscall_Detector::get_trace_vectors( Trace_Reader & trace_reader ){

    std::vector<struct svm_node *> trace_vectors;

    if ( trace_reader.has_next() ){
        Trace_Window tmp_window( _window.get_trace_length() );
        Support_Vector_Generator tmp_sv_generator( _sv_generator.size() );

        while ( trace_reader.has_next() ){
            int_fast32_t curr_syscall = trace_reader.next_syscall();
            if ( curr_syscall >= 0 ) {
                process_data_point( curr_syscall, tmp_window, tmp_sv_generator );
            }
            if ( tmp_sv_generator.full() ){
                struct svm_node * tmp_support_vector = tmp_sv_generator.get_support_vector();
                trace_vectors.push_back( tmp_support_vector );
                tmp_sv_generator.reset();
            }
        }

        // If there is remaining space in the sv_generator, but it isn't empty, fill it with the 
        // first m datapoints that can be generated from the trace window where m is the number of 
        // data points that can be added to the sv_generator before it's full.

        if ( tmp_window.full() ){

            if ( !tmp_sv_generator.empty() && fill_generator( tmp_window, tmp_sv_generator ) ){
                struct svm_node * tmp_support_vector = tmp_sv_generator.get_support_vector();
                trace_vectors.push_back( tmp_support_vector );
            }

            tmp_sv_generator.reset();

            if ( fill_generator( tmp_window, tmp_sv_generator ) ){
                struct svm_node * tmp_support_vector = tmp_sv_generator.get_support_vector();
                trace_vectors.push_back( tmp_support_vector );
                tmp_sv_generator.reset();
            }
        }
    }

    return trace_vectors;
}

// TODO: Comment this better.  I'd do it now, but I'm on a roll.

bool Syscall_Detector::fill_generator( Trace_Window & window, Support_Vector_Generator & sv_generator ){
    uint_fast32_t index = 0;

    if ( _ngram_generator->num_data_points( window ) >= sv_generator.points_until_full() ){
        // This check is somewhat paranoid, but we want to avoid problems.
        while( !sv_generator.full() && _ngram_generator->has_next( window, index ) ){
            sv_generator.add_data_point( _ngram_generator->generate_data_point( window, index++ ));
        }
    }

    return sv_generator.full();
}

struct svm_node * Syscall_Detector::get_current_vector(){

    return get_current_vector( _window );
}

struct svm_node * Syscall_Detector::get_current_vector( Trace_Window window ){

    struct svm_node * tmp_node = NULL;

    if ( window.full() ){

        Support_Vector_Generator tmp_sv_generator( _sv_generator.size() );

        while ( _ngram_generator->has_next( window ) &  ){
            tmp_sv_generator.add_data_point( _ngram_generator->generate_data_point( window ) );
        }
    }

    return svm_node;
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
