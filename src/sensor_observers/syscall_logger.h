/*
 *  File Name : syscall_logger.h
 *  
 *  Creation Date : 05-31-2016
 *
 *  Last Modified : Tue 11 Dec 2018 08:14:33 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <regex>
#include <string>
#include <thread>
#include <unordered_set>

#include "sensor_observer.h"
#include "data_streams/data_stream.h"
#include "data_records/data_record.h"

// Here we do an OS check (like in the reader factory).
// As this logger will be on the same machine as the sensors,
// and we know it's a syscall logger,
// we can define them to be <OS>_Syscall_Record where
// <OS> is the operating system this is compiled on.
// Currently, only Linux is defined, so the default behavior
// will just dump the Data_Record's raw string.

#ifdef __linux__
#include "data_records/linux/linux_syscall_record.h"
#include "sensor_data/data_patterns/syscall_patterns/linux_syscall_constants.h"
typedef Linux_Syscall_Record Syscall_Record;
#define syscall_pattern Linux_Syscall_Constants::FTRACE_REG
#else
typedef Data_Record Syscall_Record;
typedef syscall_pattern ".*"
#endif


class Syscall_Logger : public Sensor_Observer{

    public:

        Syscall_Logger();
        ~Syscall_Logger();

        // Overridden from Sensor_Observer (sensor_observer.h)

        void update();
        void update( Sensor_Data data );

        void set_observing( bool on );
        void set_processing( bool on );

        bool observing_status();
        bool processing_status();

        void start_observing();
        void start_processing();

        void stop_observing();
        void stop_processing();

        // Unique to Syscall_Logger
        
        void add_stream( Data_Stream * stream );
        void remove_stream( Data_Stream * stream );

    private:

        void process();

        void process_data_point( Sensor_Data data );

        void send_data( Syscall_Record& record );
        void process_remaining_queue();

        void clear_streams();

        std::unordered_set<Data_Stream *> streams;
        static std::regex syscall_regex;
};
