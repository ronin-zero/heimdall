#pragma once

/*
 *  File Name : detector_manager.h
 *  
 *  Creation Date : 03-08-2017
 *
 *  Last Modified : Fri 30 Jun 2017 10:17:56 AM EDT
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
#include "sensor_observers/detectors/syscall_detector.h"
#include "daemon_utils/daemonizer.h"

class Detector_Manager{

    public:

        Detector_Manager( size_t window_length, uint_fast32_t ngram_length, std::string detection_log_file_name, std::string trace_out_file_name, std::string program_name );
        Detector_Manager( size_t window_length, uint_fast32_t ngram_length, std::string detection_log_file_name, std::string trace_out_file_name, std::string program_name, uint_fast8_t arch );
        ~Detector_Manager();

        void run_detector( bool daemon_on=true );

        bool train_on_model( std::string trace_model );
        bool train_on_trace( std::string trace_file );
        bool train_model ();
        bool save_model ( std::string file_name );

        double test_file ( std::string test_file_name );

    private:

        Syscall_Sensor * sensor;
        Syscall_Logger * logger;
        Output_Stream * s_out;

        Syscall_Detector * detector;

        std::string task_name;

        bool running;

        void run_loop( bool daemon_on );
        void handle_pipe();
        
        // TODO: Determine if we need variables for the pipe name, pid, etc.

        // TODO: Determine what methods/functions are needed (start? stop? report status? etc?)
};
