/*
 *  File Name : sensor_manager.cpp
 *  
 *  Creation Date : 08-07-2016
 *
 *  Last Modified : Sat 09 Jul 2016 02:35:53 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensor_manager.h"

Sensor_Manager::Sensor_Manager( uint_fast8_t sensor_flags, std::string file_name, std::string separator ){

    sensor = Syscall_Sensor::get_instance();

    logger = new Syscall_Logger();

    s_out = new Output_Stream( file_name, sensor_flags, separator );
}

Sensor_Manager::~Sensor_Manager(){

    delete ( sensor );
    delete ( logger );
    delete ( s_out );
}

void Sensor_Manager::run_sensor( bool daemon_on ){

    std::cout << "Running this mother as ";

    daemon_on ? std::cout << "a daemon." : std::cout << "a regular process.";

    std::cout << std::endl;
}
