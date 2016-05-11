#pragma once

#include <stdint.h>
#include <fstream>
#include "sensor_data/sensor_data.h"

class Syscall_Reader{

    static Syscall_Reader * sr_instance;

public:
    virtual Syscall_Reader * get_instance() = 0;

    ~Syscall_Reader();    
    
    virtual uint_fast8_t start_reading() = 0;
    virtual uint_fast8_t stop_reading() = 0;
    virtual uint_fast8_t toggle_reading() = 0;
    virtual uint_fast8_t set_self_filter() = 0;

    virtual uint_fast8_t set_enter( bool on );
    virtual uint_fast8_t set_exit( bool on );

    virtual uint_fast8_t reading_status() = 0;

    virtual bool is_reading() = 0;

    virtual Sensor_Data read_syscall() = 0;

private:

    std::ifstream trace_pipe_stream;
};

Syscall_Reader::~Syscall_Reader() {}
