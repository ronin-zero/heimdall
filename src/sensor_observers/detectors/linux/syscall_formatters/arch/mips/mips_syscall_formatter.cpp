/*
 *  File Name : mips_syscall_formatter.cpp
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Mon 19 Jun 2017 07:23:45 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "mips_syscall_formatter.h"

MIPS_Syscall_Formatter::MIPS_Syscall_Formatter(){
    
    //  Nothing needs to be done for the constructor.
}

uint_fast32_t MIPS_Syscall_Formatter::format_syscall_num( uint_fast32_t syscall_num ){

    return syscall_num % MIPS_OFFSET;
}

uint_fast32_t MIPS_Syscall_Formatter::recover_syscall_num( uint_fast32_t formatted_syscall_num ){

    return formatted_syscall_num | MIPS_OFFSET;
}

uint_fast32_t MIPS_Syscall_Formatter::table_size() const{

    return table_size_;
}
