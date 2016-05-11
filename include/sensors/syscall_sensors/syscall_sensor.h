#pragma once

#include <stdint.h>

#include "sensors/sensor.h"
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"
#include "sensor_data/sensor_data.h"

class Syscall_Sensor:public Sensor{

const uint_fast8_t SENSING_ON = 0x01;
const uint_fast8_t FILTER_SELF = 0x02;
const uint_fast8_t SYS_ENTER = 0x04;
const uint_fast8_t SYS_EXIT = 0x08;

const uint_fast8_t SENSOR_DEFAULT = SENSING_ON | FILTER_SELF | SYS_ENTER;

static Syscall_Sensor * ss_instance;

public:
    static Syscall_Sensor * get_instance();
    uint_fast8_t configure( uint_fast8_t flags );
    ~Syscall_Sensor();
    Sensor_Data sense_data();
    uint_fast8_t sensing_status();
    bool is_sensing();
    void sense();
    uint_fast8_t toggle_sensing();
    void notify_observers( Sensor_Data data );
    void notify_observers();

private:
    Syscall_Sensor();
    uint_fast8_t set_exit( bool on );
    uint_fast8_t set_enter( bool on );
//    uint_fast8_t set_self_filter( bool filter_self );

    uint_fast8_t status;
    Syscall_Reader * reader;

};
