/*
 *  File Name : sensor_manager.cpp
 *  
 *  Creation Date : 07-08-2016
 *
 *  Last Modified : Mon 13 Mar 2017 06:33:20 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensor_manager.h"

Sensor_Manager::Sensor_Manager( uint_fast8_t sensor_flags, std::string file_name, std::string separator, std::string program_name  ){

    sensor = Syscall_Sensor::get_instance();

    task_name = program_name;

    logger = new Syscall_Logger();

    s_out = new Output_Stream( file_name, sensor_flags, separator );

    logger->add_stream( s_out );
    sensor->attach_observer( logger );
}

Sensor_Manager::~Sensor_Manager(){

    delete ( sensor ); 
    delete ( logger );
    delete ( s_out );
}

void Sensor_Manager::run_sensor( bool daemon_on ){

    if ( daemon_on )
    {
        Daemonizer::launch_daemon( task_name );
    }

    // We want to start the observing and processing threads before we begin sensing.
    // That way, their thread IDs exist and can get added into the filter.
    // There will be nothing to observe or process, but that should be alright.
    // The processing thread yields if there is no data in the queue.

    logger->start_observing(); 
    logger->start_processing();
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

void Sensor_Manager::run_loop( bool daemon_on ){

    while ( running )
    {
        if ( daemon_on )
        {
            handle_pipe();
        }
        else
        {
            string input;

            std::cout << "Enter STOP or stop to stop the sensor." << std::endl;
            std::cin >> input;

            running = !( input == "STOP" || input == "stop" || input == "Stop" );
        }
    }
}

void Sensor_Manager::handle_pipe(){

    std::string pipe_name = "/var/run/sensor.pipe";

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
