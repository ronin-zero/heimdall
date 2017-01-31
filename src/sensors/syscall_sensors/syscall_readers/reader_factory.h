/*
 *  File Name : reader_factory.h
 *  
 *  Creation Date : 05-09-2016
 *
 *  Last Modified : Tue 31 Jan 2017 03:34:57 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

// Check for OS (Currently, only Linux is supported).

#ifdef __linux__
#include "linux_syscall_reader.h"
typedef Linux_Syscall_Reader Reader;
#else
#include "syscall_reader.h"
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
