#pragma once

#include <cstdint>

// Check for OS (Currently, only Linux is supported).

#ifdef __linux__
#include "sensors/syscall_sensors/syscall_readers/linux_syscall_reader.h"
typedef Linux_Syscall_Reader Reader;
#else
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"
typedef Syscall_Reader Reader;
#endif

class Reader_Factory{

    public:
        Reader_Factory( uint_fast8_t flags=READER_DEFAULT );
        ~Reader_Factory() {}

        uint_fast8_t set_status( uint_fast8_t flags );

        Reader * create_reader();

    protected:
        uint_fast8_t status;

};
