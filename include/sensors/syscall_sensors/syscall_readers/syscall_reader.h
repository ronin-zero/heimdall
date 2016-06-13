#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
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

        //~Syscall_Reader() {}

        // This is the ugliest line of terrible code I've written in my life.  If anyone knows a better way, please let me know.
        // More information: this doesn't compile with the way I've set the warning flags (-Wall -Werror -Wextra -pedantic, etc.)
        // and I don't want to turn them off.  The problem is it accepts a uint_fast8_t as an argument, but this was just supposed
        // to be the template for derived classes.  Ideally, it would be virtual, but it must be static in order to keep it as
        // a singleton. I have a few ideas of how to fix it:
        //
        // 1) Maybe don't make this a pure virtual abstract class/interface.
        // 2) Maybe don't define THIS method in the interface; really as far as I know, only Linux syscall readers have to be a singleton.
        // 3) Make a dummy constructor?

        static Syscall_Reader * get_instance( uint_fast8_t flags=READER_DEFAULT ) { return ( flags == READER_DEFAULT ? NULL : NULL ); }

        ~Syscall_Reader(){}

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

    protected:


        uint_fast8_t status;

        string os;
        string data_type = "syscall";

        std::ifstream trace_pipe_stream;
};
