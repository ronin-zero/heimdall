/*
 *  File Name : syscall_formatter.h
 *  
 *  Creation Date : 12-22-2016
 *
 *  Last Modified : Mon 31 Jul 2017 12:27:34 PM EDT
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

        virtual uint_fast32_t table_size() const;

    private:

        const uint_fast32_t table_size_ = 400;
};
