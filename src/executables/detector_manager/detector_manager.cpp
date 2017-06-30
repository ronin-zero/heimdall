/*
 *  File Name : detector_manager.cpp
 *  
 *  Creation Date : 03-08-2017
 *
 *  Last Modified : Fri 30 Jun 2017 10:18:01 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "detector_manager.h"

Detector_Manager::Detector_Manager( size_t window_length, uint_fast32_t ngram_length, std::string detection_log_file_name, std::string trace_out_file_name, std::string program_name ){

    sensor = Syscall_Sensor::get_instance();

    detector = new Syscall_Detector( window_length, ngram_length, detection_log_file_name );

    task_name = program_name;

    logger = new Syscall_Logger();

    // The logger should just use 0xcc which will record task name, PID, timestamp, and syscall number.
    // The separator should stay the default, which is a comma.
    // The file name is now trace_out_file_name.

    s_out = new Output_Stream( trace_out_file_name, 0xcc, ",");

    logger->add_stream( s_out );
    sensor->attach_observer( logger );
    sensor->attach_observer( detector );
}

// TODO: This is sloppy as hell but i have 4 hours.  Definitely do this better later.

Detector_Manager::Detector_Manager( size_t window_length, uint_fast32_t ngram_length, std::string detection_log_file_name, std::string trace_out_file_name, 
        std::string program_name, uint_fast8_t arch)
{
    sensor = Syscall_Sensor::get_instance();

    detector = new Syscall_Detector( window_length, ngram_length, detection_log_file_name, arch );

    task_name = program_name;

    logger = new Syscall_Logger();

    // The logger should just use 0xcc which will record task name, PID, timestamp, and syscall number.
    // The separator should stay the default, which is a comma.
    // The file name is now trace_out_file_name.

    s_out = new Output_Stream( trace_out_file_name, 0xcc, ",");

    logger->add_stream( s_out );
    sensor->attach_observer( logger );
    sensor->attach_observer( detector );
}

Detector_Manager::~Detector_Manager(){

    delete ( sensor ); 
    delete ( logger );
    delete ( s_out );
    delete ( detector );
}

bool Detector_Manager::train_on_model( std::string trace_model ){

    return detector->train_from_saved_model( trace_model );
}

bool Detector_Manager::train_on_trace( std::string trace_file ){ 

    return detector->train_from_trace( trace_file );
}

bool Detector_Manager::train_model(){

    return detector->train_model();
}

bool Detector_Manager::save_model( std::string file_name ){
    
    return detector->save_model( file_name );
}

double Detector_Manager::test_file( std::string test_file_name ){
    
    return detector->test_trace_file( test_file_name );
}

void Detector_Manager::run_detector( bool daemon_on ){

    if ( daemon_on )
    {
        Daemonizer::launch_daemon( task_name );
    }

    logger->start_observing();
    logger->start_processing();

    detector->start_observing();
    detector->start_processing();

    running = ( sensor->start_sensing() & SENSING_ON );

    if ( !running )
    {
        std::cerr << "ERROR: Could not start sensor (check your permissions -- you may need root/sudo)." << std::endl;
    }
    else
    {
        run_loop( daemon_on );
    }
}

void Detector_Manager::run_loop( bool daemon_on ){

    while ( running )
    {
        if ( daemon_on )
        {
            handle_pipe();
        }
        else
        {
            string input;

            std::cout << "Enter STOP or stop to stop the detector." << std::endl;
            std::cin >> input;

            running = !( input == "STOP" || input == "stop" || input == "Stop" );
        }
    }
}

void Detector_Manager::handle_pipe(){

    std::string pipe_name = "/var/run/detector.pipe";

    int32_t fd = open( pipe_name.c_str(), O_RDONLY );

    uint32_t buffsize = 32; // CHECK:  Apologies for magic number.
    char* buf = new char[ buffsize ];

    if ( !read( fd, buf, buffsize ) )
    {
        running = false;
    }
    else
    {
        std::string input( buf );

        running = ( input != "STOP" );

        close (fd);
    }

    delete[] buf;
}
