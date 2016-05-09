#pragma once

#include <stdint.h>

#include "../sensor.h"
#include "../../sensor_data/sensor_data.h"

class Syscall_Sensor:public Sensor{

const fast_uint8_t SENSING_ON = 0x01;
const fast_uint8_t FILTER_SELF = 0x02;
const fast_uint8_t SYS_ENTER = 0x04;
const fast_uint8_t SYS_EXIT = 0x08;

const fast_uint8_t SENSOR_DEFAULT = SENSING_ON | FILTER_SELF | SYS_ENTER;

static Syscall_Sensor *ss_instance;

public:
    static Syscall_Sensor * get_instance();
    fast_uint8_t configure( fast_uint8_t flags );

private:
    Syscall_Sensor();
    fast_uint8_t set_exit( bool on );
    fast_uint8_t set_enter( bool on );
//    fast_uint8_t set_self_filter( bool filter_self );

    fast_uint8_t status;
    Syscall_Reader reader;

}

