/*
 *  File Name : mips_n64_syscall_formatter.cpp
 *  
 *  Creation Date : 08-01-2017
 *
 *  Last Modified : Tue 01 Aug 2017 02:02:43 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "mips_n64_syscall_formatter.h"

MIPS_n64_Syscall_Formatter::MIPS_n64_Syscall_Formatter() {

    MIPS_OFFSET = MIPS_N64_OFFSET;
    table_size_ = MIPS_N64_TABLE_SIZE;
}
