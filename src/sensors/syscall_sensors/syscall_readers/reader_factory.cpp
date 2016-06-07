/*
 *  File Name : reader_factory.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Mon 06 Jun 2016 06:48:14 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */
#include "sensors/syscall_sensors/syscall_readers/reader_factory.h"

// Check for operating system here (So far, only Linux is defined).

#ifdef __linux__
#include "sensors/syscall_sensors/syscall_readers/linux_syscall_reader.h"
typedef Linux_Syscall_Reader Reader;
#else
typedef Syscall_Reader Reader;
#endif

Reader_Factory::Reader_Factory( uint_fast8_t flags ){

    status = flags;
}

uint_fast8_t Reader_Factory::set_status( uint_fast8_t flags ){

    status = flags;
    return status;
}

Syscall_Reader * Reader_Factory::create_reader(){

    Syscall_Reader * concrete_reader = Reader::get_instance( status );
    return concrete_reader;
}
