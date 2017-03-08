/*
 *  File Name : mips_syscall_formatter.h
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Wed 08 Mar 2017 12:27:58 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint> 

#include "sensor_observers/detectors/linux/syscall_formatters/syscall_formatter.h"


class MIPS_Syscall_Formatter : public Syscall_Formatter{

    public:

        MIPS_Syscall_Formatter();
        ~MIPS_Syscall_Formatter() {}

        uint_fast32_t format_syscall_num( uint_fast32_t syscall_num );
        uint_fast32_t recover_syscall_num( uint_fast32_t formatted_syscall_num );

    private:

        static const uint_fast32_t MIPS_OFFSET  =   4000;

        /*  CHECK: The assumption of 4000 for the MIPS offset is consistent with mips_o32 architecture.
         *
         *  Source: https://w3challs.com/syscalls/?arch=mips_o32
         *
         *  4000 seems to be the starting (lowest) system call number for MIPS targets in OpenWrt,
         *  but only malta (both big and little endian) has been tested (not thoroughly!).  If this leads
         *  to an issue with other MIPS targets, this could be the culprit.
         */

};
