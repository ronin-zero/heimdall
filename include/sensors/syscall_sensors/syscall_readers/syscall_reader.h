#pragma once

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>

#include "sensor_data/sensor_data.h"

static const uint_fast8_t READING_ON       = 0x01; // Note that this is the same value as SENSING_ON.
static const uint_fast8_t FILTER_SELF      = 0x02;
static const uint_fast8_t SYS_ENTER        = 0x04;
static const uint_fast8_t SYS_EXIT         = 0x08;
static const uint_fast8_t READER_DEFAULT   = READING_ON | SYS_ENTER | FILTER_SELF;

class Syscall_Reader{

    public:

        ~Syscall_Reader();

        static Syscall_Reader * get_instance( uint_fast8_t flags=READER_DEFAULT );
        virtual uint_fast8_t start_reading() = 0;
        virtual uint_fast8_t stop_reading() = 0;
        virtual uint_fast8_t toggle_reading() = 0;
        virtual uint_fast8_t set_reading_on( bool on ) = 0;
        virtual uint_fast8_t set_self_filter( bool on=true ) = 0;

        virtual uint_fast8_t set_enter( bool on );
        virtual uint_fast8_t set_exit( bool on );

        virtual uint_fast8_t reading_status() = 0;

        virtual bool is_reading() = 0;

        virtual Sensor_Data read_syscall() = 0;

    protected:

        uint_fast8_t status;

        std::ifstream trace_pipe_stream;
};

Syscall_Reader::~Syscall_Reader() {}



