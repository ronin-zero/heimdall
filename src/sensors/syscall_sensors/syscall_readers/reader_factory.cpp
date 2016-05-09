/*
 *  File Name : reader_factory.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Mon 09 May 2016 03:53:08 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_reader.h"

Reader_Factory::Reader_Factory(){

    status = SENSOR_DEFAULT;
}

Reader_Factory::Reader_Factory( fast_uint8_t flags){

    status = flags;
}

fast_uint8_t set_status( fast_uint8_t flags ){

    status = flags;
    return status;
}

Syscall_Reader * Reader_Factory::create_reader( fast_uint8_t flags = SENSOR_DEFAULT ){

    Syscall_Reader* reader = NULL;

#ifdef __linux__
    reader = new Linux_Syscall_Reader( flags );
#endif
    return reader;
}
