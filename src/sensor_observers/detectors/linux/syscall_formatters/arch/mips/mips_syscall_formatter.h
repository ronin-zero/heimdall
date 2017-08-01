/*
 *  File Name : mips_syscall_formatter.h
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Tue 01 Aug 2017 12:15:10 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint> 

#include "sensor_observers/detectors/linux/syscall_formatters/syscall_formatter.h"

#define MIPS_O32_OFFSET     4000
#define MIPS_N64_OFFSET     5000
#define MIPS_N32_OFFSET     6000
#define MIPS_GEN_OFFSET     1000 // In the event that the EABI/OABI can't be detected

#define MIPS_032_TABLE_SIZE 360
#define MIPS_N64_TABLE_SIZE 320
#define MIPS_N32_TABLE_SIZE 324
#define MIPS_GEN_TABLE_SIZE 360 // Use the largest of the three table sizes when ABI unknown


class MIPS_Syscall_Formatter : public Syscall_Formatter {

    public:

        MIPS_Syscall_Formatter();
        ~MIPS_Syscall_Formatter() {}

        uint_fast32_t format_syscall_num( uint_fast32_t syscall_num );
        uint_fast32_t recover_syscall_num( uint_fast32_t formatted_syscall_num );
        uint_fast32_t table_size() const;

    protected:

        uint_fast32_t MIPS_OFFSET;
        uint_fast32_t table_size_;
};
