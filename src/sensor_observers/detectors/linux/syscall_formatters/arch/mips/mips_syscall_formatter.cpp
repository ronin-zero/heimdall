/*
 *  File Name : mips_syscall_formatter.cpp
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Tue 01 Aug 2017 01:42:52 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "mips_syscall_formatter.h"

MIPS_Syscall_Formatter::MIPS_Syscall_Formatter(){

    MIPS_OFFSET = 0;
    table_size_ = MIPS_GEN_TABLE_SIZE;
}

uint_fast32_t MIPS_Syscall_Formatter::format_syscall_num( uint_fast32_t syscall_num ){

    return syscall_num % MIPS_GEN_OFFSET;
}

uint_fast32_t MIPS_Syscall_Formatter::recover_syscall_num( uint_fast32_t formatted_syscall_num ){

    return ( formatted_syscall_num % MIPS_GEN_OFFSET ) + MIPS_OFFSET;
}

uint_fast32_t MIPS_Syscall_Formatter::table_size() const{

    return table_size_;
}
