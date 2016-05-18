/*
 *  File Name : reader_factory.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Tue 17 May 2016 09:09:57 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */
#include "sensors/syscall_sensors/syscall_readers/reader_factory.h"
#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"

// Check for operating system here (So far, only Linux is defined).

#ifdef __linux__
#include "sensors/syscall_sensors/syscall_readers/linux_syscall_reader.h"
typedef Linux_Syscall_Reader Reader;
#else
typedef Syscall_Reader Reader;
#endif

Reader_Factory::Reader_Factory( uint_fast8_t flags){

    status = flags;
}

uint_fast8_t Reader_Factory::set_status( uint_fast8_t flags ){

    status = flags;
    return status;
}

Syscall_Reader * Reader_Factory::create_reader( uint_fast8_t flags ){

    Syscall_Reader * reader = Reader::get_instance( flags );
    return reader;
}
