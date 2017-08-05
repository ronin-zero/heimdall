/*
 *  File Name : syscall_formatter.cpp
 *  
 *  Creation Date : 12-22-2016
 *
 *  Last Modified : Fri 04 Aug 2017 09:56:37 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_formatter.h"

Syscall_Formatter::Syscall_Formatter(){

    // Nothing needs to be done for the constructor.
}

// Simply return the original number modded by the table_size_.
uint_fast32_t Syscall_Formatter::format_syscall_num( uint_fast32_t syscall_num ){

    return syscall_num % table_size_;
}

// Return the original number.
uint_fast32_t Syscall_Formatter::recover_syscall_num( uint_fast32_t formatted_syscall_num ){

    return formatted_syscall_num;
}

uint_fast32_t Syscall_Formatter::table_size() const{

    return table_size_;
}
