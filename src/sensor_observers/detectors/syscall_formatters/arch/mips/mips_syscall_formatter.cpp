/*
 *  File Name : mips_syscall_formatter.cpp
 *  
 *  Creation Date : 26-12-2016
 *
 *  Last Modified : Mon 26 Dec 2016 11:20:08 PM EST
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
