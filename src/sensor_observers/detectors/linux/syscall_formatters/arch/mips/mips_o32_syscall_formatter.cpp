/*
 *  File Name : mips_o32_syscall_formatter.cpp
 *  
 *  Creation Date : 07-31-2017
 *
 *  Last Modified : Tue 01 Aug 2017 12:16:14 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_o32_syscall_formatter.h"

MIPS_o32_Syscall_Formatter::MIPS_o32_Syscall_Formatter() {

    MIPS_OFFSET = MIPS_O32_OFFSET;
    table_size_ = MIPS_O32_TABLE_SIZE;
}
