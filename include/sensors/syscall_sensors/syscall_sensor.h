#pragma once

#include <cstdint>

#include "sensors/sensor.h"
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"
#include "sensor_data/sensor_data.h"

static const uint_fast8_t SENSOR_DEFAULT = READER_DEFAULT;

class Syscall_Sensor:public Sensor{

    // This needs to be declared here for this to work.
    // Originally, it was protected.  That won't work.

    static Syscall_Sensor * ss_instance;

    public:

        // As with the "get_instance" method of "syscall_reader," this is an ugly, ugly fix.
        // See lines 24~32 of ./syscall_readers/syscall_reader.h for the rationale and if you know a better
        // workaround, please let me know.

        static Syscall_Sensor * get_instance( uint_fast8_t flags=SENSOR_DEFAULT );
        virtual ~Syscall_Sensor();

        bool is_sensing();

        uint_fast8_t set_sensing( bool on );
        uint_fast8_t sensing_status();
        
        // CHECK: Shawn feels this is unnecessary.

        //uint_fast8_t toggle_sensing();
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

        Sensor_Data * sense_data();

        void sense();
        void notify_observers();
        void push_data( Sensor_Data data );
        void process_remaining_queue();

};
