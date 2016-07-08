#pragma once

/*
 *  File Name : sensor_manager.h
 *  
 *  Creation Date : 08-07-2016
 *
 *  Last Modified : Fri 08 Jul 2016 06:24:18 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <cstdint>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensor_observers/syscall_logger.h"
#include "sensor_observers/data_streams/output_stream.h"

class Sensor_Manager{

    public:

        Sensor_Manager( uint_fast8_t sensor_flags, std::string file_name, std::string separator, bool daemon_on );
        ~Sensor_Manager();

    private:

        Syscall_Sensor * sensor;
        Syscall_Logger * logger;
        Output_Stream * s_out;

        bool daemonized;
        
        // TODO: Determine if we need variables for the pipe name, pid, etc.

        // TODO: Determine what methods/functions are needed (start? stop? report status? etc?)
}
