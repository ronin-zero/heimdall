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

        bool is_sensing();

        uint_fast8_t set_sensing( bool on );
        uint_fast8_t sensing_status();
        uint_fast8_t toggle_sensing();
        uint_fast8_t start_sensing();
        uint_fast8_t stop_sensing();

        // These are unique to Syscall_Sensor

        uint_fast8_t set_exit( bool on );
        uint_fast8_t set_enter( bool on );
        uint_fast8_t set_self_filter( bool on=true );

        uint_fast8_t configure( uint_fast8_t flags );

    protected:
        
        Syscall_Sensor( uint_fast8_t flags=SENSOR_DEFAULT );

        Syscall_Reader * reader;

        static Syscall_Sensor * ss_instance;

        Sensor_Data * sense_data();
        void sense();
        void notify_observers();
        void push_data( Sensor_Data data );
        void process_remaining_queue();

};
