/*
 *  File Name : syscall_logger.h
 *  
 *  Creation Date : 31-05-2016
 *
 *  Last Modified : Tue 07 Jun 2016 04:01:00 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <string>
#include <thread>
#include <unordered_set>

#include "sensor_observers/sensor_observer.h"
#include "sensor_observers/data_streams/data_stream.h"
#include "sensor_observers/data_records/data_record.h"

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

        void send_data( Data_Record record );
        void process();
        void process_remaining_queue();

        std::unordered_set<Data_Stream *> streams;
};
