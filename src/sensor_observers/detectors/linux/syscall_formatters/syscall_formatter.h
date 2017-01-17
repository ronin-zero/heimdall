/*
 *  File Name : syscall_formatter.h
 *  
 *  Creation Date : 22-12-2016
 *
 *  Last Modified : Thu 22 Dec 2016 07:37:57 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

class Syscall_Formatter{

    public:

        Syscall_Formatter();
        virtual ~Syscall_Formatter() {}

        virtual uint_fast32_t format_syscall_num( uint_fast32_t syscall_num );
        virtual uint_fast32_t recover_syscall_num( uint_fast32_t formatted_syscall_num );
};