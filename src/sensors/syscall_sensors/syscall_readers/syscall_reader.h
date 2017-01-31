/*
 *  File Name : syscall_reader.h
 *  
 *  Creation Date : 05-09-2016
 *
 *  Last Modified : Tue 31 Jan 2017 03:36:10 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

#include "sensor_data/sensor_data.h"

using std::string;

static const uint_fast8_t READING_ON       = 0x01; // CHECK: Note that this is the same value as SENSING_ON.  May need to change it...
static const uint_fast8_t FILTER_SELF      = 0x02;
static const uint_fast8_t SYS_ENTER        = 0x04;
static const uint_fast8_t SYS_EXIT         = 0x08;
static const uint_fast8_t READER_DEFAULT   = SYS_ENTER | FILTER_SELF;

class Syscall_Reader{

    public:

        static Syscall_Reader * get_instance();
        static Syscall_Reader * get_instance( uint_fast8_t flags );

        virtual ~Syscall_Reader(){}

        virtual uint_fast8_t set_reading( bool on ) = 0;

        // CHECK: Shawn feels these toggle methods are
        // unnecessary.  I agree.
        
        // virtual uint_fast8_t toggle_reading() = 0;
        virtual uint_fast8_t start_reading() = 0;
        virtual uint_fast8_t stop_reading() = 0;

        virtual uint_fast8_t set_self_filter( bool on=true ) = 0;
        
        virtual uint_fast8_t set_enter( bool on ) = 0;
        virtual uint_fast8_t set_exit( bool on ) = 0;

        virtual uint_fast8_t reading_status() = 0;

        virtual bool is_reading() = 0;

        virtual Sensor_Data * read_syscall() = 0;

        virtual uint_fast8_t configure( uint_fast8_t flags ) = 0;

        virtual void update_filter() = 0;

    protected:

        uint_fast8_t status;

        string os;
        string data_type = "syscall";

        std::ifstream trace_pipe_stream;
};
