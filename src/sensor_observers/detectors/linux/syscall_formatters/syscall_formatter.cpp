/*
 *  File Name : syscall_formatter.cpp
 *  
 *  Creation Date : 12-22-2016
 *
 *  Last Modified : Mon 31 Jul 2017 12:27:53 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

// TODO: Comment this class properly.

#include "syscall_formatter.h"

Syscall_Formatter::Syscall_Formatter(){

}

uint_fast32_t Syscall_Formatter::format_syscall_num( uint_fast32_t syscall_num ){

    return syscall_num;
}

uint_fast32_t Syscall_Formatter::recover_syscall_num( uint_fast32_t formatted_syscall_num ){

    return formatted_syscall_num;
}

uint_fast32_t Syscall_Formatter::table_size() const{

    return table_size_;
}
