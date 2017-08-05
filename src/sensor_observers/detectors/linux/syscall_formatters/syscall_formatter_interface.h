/*
 *  File Name : syscall_formatter.h
 *  
 *  Creation Date : 12-22-2016
 *
 *  Last Modified : Fri 04 Aug 2017 10:10:02 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

class Syscall_Formatter_Interface{

    public:

        virtual ~Syscall_Formatter_Interface() {}

        virtual uint_fast32_t format_syscall_num( uint_fast32_t syscall_num ) = 0;
        virtual uint_fast32_t recover_syscall_num( uint_fast32_t formatted_syscall_num ) = 0;

        virtual uint_fast32_t table_size() const = 0;

};
