#pragma once

#include <stdint.h>
#include <fstream>
#include "sensor_data/Sensor_Data.h"

class Syscall_Reader{

    static Syscall_Reader * sr_instance;

public:
    virtual Syscall_Reader * get_instance() = 0;
    
    virtual fast_uint8_t start_reading() = 0;
    virtual fast_uint8_t stop_reading() = 0;
    virtual fast_uint8_t toggle_reading() = 0;
    virtual fast_uint8_t set_self_filter() = 0;

    virtual fast_uint8_t reading_status() = 0;

    virtual bool is_reading() = 0;

    virtual Sensor_Data read_syscall() = 0;

private:

    ifstream trace_pipe_stream;
}

Syscall_Reader::~Syscall_Reader() {}
