#pragma once

// Check OS.  Only Linux System Call records are defined at this point.
#ifdef __linux__
#include "sensor_observers/data_records/linux/syscall_record.h"
#else
#include "sensor_observers/data_records/data_record.h"
#endif

#include <cstdint>

#include "sensor_data/sensor_data.h"

class Record_Factory{

    public: 

    protected:

        uint_fast8_t flags;

};
