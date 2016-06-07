#pragma once

#include <cstdint>

#include "sensor_observers/data_records/linux/linux_syscall_record.h"
#include "sensor_observers/data_records/data_record.h"
#include "sensor_data/sensor_data.h"

//  REFACTOR: I'm not a huge fan of this class.  It seems like it might be excessive.
//  The idea is that the "Syscall_Logger" just passes Sensor_Data objects to its
//  Output_Streams.  The Output_Streams take a Sensor_Data object and produce a
//  "Data_Record" object that has behavior defined for how it should format itself
//  when passed to a true ostream (cout, an ofstream, etc.).
//  I just can't think of a better way to handle this.

class Record_Factory{

    public: 

        Record_Factory( Sensor_Data data, uint_fast8_t out_flags=ALL );

        Data_Record produce_record( Sensor_Data data );

        uint_fast8_t set_flags( uint_fast8_t new_flags );
        uint_fast8_t get_flags();

    protected:

        uint_fast8_t flags;
};

uint_fast8_t Record_Factory::set_flags( uint_fast8_t new_flags ){

    flags = new_flags;
    
    return flags;
}

uint_fast8_t Record_Factory::get_flags(){
    
    return flags;
}
