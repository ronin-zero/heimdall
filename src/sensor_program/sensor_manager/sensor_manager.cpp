/*
 *  File Name : sensor_manager.cpp
 *  
 *  Creation Date : 08-07-2016
 *
 *  Last Modified : Mon 11 Jul 2016 08:53:37 PM EDT
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

    std::cout << "Running this as ";

    daemon_on ? std::cout << "a daemon." : std::cout << "a regular process.";

    if ( daemon_on )
    {
        Daemonizer::launch_daemon( task_name );
    }

    logger->start_observing();
    logger->start_processing();

    running = ( sensor->start_sensing() & SENSING_ON );

    if ( !running )
    {
        std::cerr << "ERROR: Could not start sensor (check your permissions -- you made need root/sudo)." << std::endl;
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

            running = ( input == "STOP" || input == "stop" || input == "Stop" );

        }
    }
}

void Sensor_Manager::handle_pipe(){

    std::string pipe_name = "/tmp/syscall_sensor.fifo";

    std::cout << "(From Sensor_Manager) pipe_name = " << pipe_name << std::endl;
    int32_t fd = open( pipe_name.c_str(), O_RDONLY );

    uint32_t buffsize = 32; // CHECK:  Apologies for magic number.
    char* buf = new char[ buffsize ];

    std::cout << "ABOUT TO READ" << std::endl;
    read( fd, buf, buffsize );
    std::cout << "DONE READING.  WE READ: ";

    std::string input( buf );

    std::cout << input << std::endl;

    running = ( input != "STOP" );

    close (fd);
    free (buf);

}
