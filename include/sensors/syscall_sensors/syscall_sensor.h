#pragma once

#include <stdint.h>

#include "sensors/sensor.h"
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"
#include "sensor_data/sensor_data.h"

static const uint_fast8_t SENSING_ON = READING_ON;
//static const uint_fast8_t FILTER_SELF = 0x02;
//static const uint_fast8_t SYS_ENTER = 0x04;
//static const uint_fast8_t SYS_EXIT = 0x08;
static const uint_fast8_t SENSOR_DEFAULT = READER_DEFAULT;

class Syscall_Sensor:public Sensor{

    public:
        static Syscall_Sensor * get_instance( uint_fast8_t flags=SENSOR_DEFAULT );
        ~Syscall_Sensor();
        uint_fast8_t configure( uint_fast8_t flags );
        Sensor_Data sense_data();
        uint_fast8_t sensing_status();
        bool is_sensing();
        void sense();

        uint_fast8_t set_sensing( bool on );
        uint_fast8_t toggle_sensing();
        uint_fast8_t start_sensing();
        uint_fast8_t stop_sensing();

        uint_fast8_t set_exit( bool on );
        uint_fast8_t set_enter( bool on );
        uint_fast8_t set_self_filter( bool on=true );

        void notify_observers();


    protected:
        Syscall_Sensor( uint_fast8_t flags=SENSOR_DEFAULT );

        static Syscall_Sensor * ss_instance;


        uint_fast8_t status;
        Syscall_Reader * reader;
};
