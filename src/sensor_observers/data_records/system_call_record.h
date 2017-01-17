/*
 *  File Name : system_call_record.h
 *  
 *  Creation Date : 23-12-2016
 *
 *  Last Modified : Fri 23 Dec 2016 02:51:34 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

        /*  This interface can be implemented by system call records
         *  from any operating system to allow for cross-platform extension.
         *
         *  For instance, how the system call number is recovered may vary across operating 
         *  systems (or other factors).  Implementing system call record classes are
         *  required to provide their own definition for this behavior.
         *
         *  For now, the only functionalities needed (or that seem like they will
         *  be needed in the foreseeable future) are:
         *
         *  get_syscall_number()
         *
         *  and
         *
         *  get_pid_number()
         */

class System_Call_Record{

    public:

       virtual uint_fast32_t get_pid_num() const =0;
       virtual uint_fast32_t get_syscall_num() const =0;

};