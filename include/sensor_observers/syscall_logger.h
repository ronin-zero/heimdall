/*
 *  File Name : syscall_logger.h
 *  
 *  Creation Date : 31-05-2016
 *
 *  Last Modified : Tue 31 May 2016 07:33:42 PM EDT
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

class Syscall_Logger : public Sensor_Observer{

    public:

        Syscall_Logger();
        ~Syscall_Logger();

        // Overridden from Sensor_Observer (sensor_observer.h)

        void update();
        void update( Sensor_Data data );

        uint_fast8_t set_observing( bool observe );
        uint_fast8_t set_processing( bool process );

        uint_fast8_t observing_status();
        uint_fast8_t processing_status();
                                        
        uint_fast8_t toggle_observing();
        uint_fast8_t toggle_processing();

        uint_fast8_t start_observing();
        uint_fast8_t start_processing();

        uint_fast8_t stop_observing();
        uint_fast8_t stop_processing();

        // Unique to Syscall_Logger
        
        void add_stream( Data_Stream * stream );
        void remove_stream( Data_Stream * stream );

    private:

        std::unordered_set<Data_Stream *> streams;


};
