#pragma once

#include <stdint.h>
#include "syscall_reader.h"

class Reader_Factory{

public:
    Reader_Factory( fast_uint8_t flags=SENSOR_DEFAULT );
    ~Reader_Factory();
    
    fast_uint8_t set_status( fast_uint8_t flags );

    Syscall_Reader * create_reader( fast_uint8_t flags=SENSOR_DEFAULT);

}
