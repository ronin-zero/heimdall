#pragma once

#include <cstdint>

#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"

class Reader_Factory{

    public:
        Reader_Factory( uint_fast8_t flags=READER_DEFAULT );
        ~Reader_Factory();

        uint_fast8_t set_status( uint_fast8_t flags );

        Syscall_Reader * create_reader();

    protected:
        uint_fast8_t status;

};
