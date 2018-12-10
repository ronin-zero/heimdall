/*
 *  File Name : linux_syscall_record.cpp
 *  
 *  Creation Date : 06-01-2016
 *
 *  Last Modified : Mon 10 Dec 2018 03:16:43 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "linux_syscall_record.h"

Linux_Syscall_Record::Linux_Syscall_Record( const std::smatch& matches, uint_fast8_t settings_flags, std::string sep )
: Data_Record( matches, settings_flags, sep ){
    // Nothing to do here for now.
}

std::string Linux_Syscall_Record::get_task() const{

    return get_field( Linux_Syscall_Constants::TASK_INDEX );
}

std::string Linux_Syscall_Record::get_pid() const{

    return get_field( Linux_Syscall_Constants::PID_INDEX );
}

std::string Linux_Syscall_Record::get_cpu() const{

    return get_field( Linux_Syscall_Constants::CPU_INDEX );
}

std::string Linux_Syscall_Record::get_trace_flags() const{

    return get_field( Linux_Syscall_Constants::TRACE_FLAGS_INDEX );
}

std::string Linux_Syscall_Record::get_timestamp() const{

    return get_field( Linux_Syscall_Constants::TIMESTAMP_INDEX );
}

std::string Linux_Syscall_Record::get_syscall() const{

    return get_field( Linux_Syscall_Constants::SYSCALL_INDEX );
}

std::string Linux_Syscall_Record::get_syscall_args() const{

    return get_field( Linux_Syscall_Constants::ARGS_INDEX );
}

//  At present, even on a 64 bit system, the maximum PID value 
//  is 2^22 and a PID will never be negative.  As such, I calculate
//  the PID value as a uint_fast32_t.

uint_fast32_t Linux_Syscall_Record::get_pid_num() const{

    return ASCII_Operations::to_uint( get_pid() );
}

//  NOTE: I've encountered some cases where the syscall number is -1
//  on x86_64 Linux (on motherbase).  This field is now returned as
//  a signed int until I can figure out what's going on and how
//  best to proceed.

int_fast32_t Linux_Syscall_Record::get_syscall_num() const{

    return ASCII_Operations::to_int( get_syscall() );
}

uint_fast16_t Linux_Syscall_Record::get_cpu_num() const{

    return ASCII_Operations::to_uint( get_cpu() );
}

double Linux_Syscall_Record::get_timestamp_num() const{

    return ASCII_Operations::to_floating_point( get_timestamp() );
}

// Protected methods

// This will print out the fields specified by the flags,
// separated by commas.

std::string Linux_Syscall_Record::get_field( size_t field_num ){

    return ( field_num < record_fields.size() ? record_fields[ field_num ] : "UNDEF_" + std::to_string( field_num ) + "_" + std::to_string( record_fields.size() ) );
}

void Linux_Syscall_Record::print( ostream& s_out ) const{

    // We will keep delim as an empty character.
    // If there are no fields to be printed, we
    // will not need to separate them with anything.
    // Once a field appears, we will change it to
    // whatever our separator is (comma by default: ',').
    // This way, we won't need to check a nested 
    // if-statement every single time we print a new field.

    bool needs_sep = false;

    if ( flags & PROCESS_NAME )
    {
        s_out << get_task();

        needs_sep = true;
    }

    if ( flags & PID )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_pid();

        needs_sep = true;
    }

    if ( flags & CPU )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_cpu();

        needs_sep = true;
    }

    if ( flags & TRACE_FLAGS )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_trace_flags();

        needs_sep = true;
    }

    if ( flags & TIMESTAMP )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_timestamp();

        needs_sep = true;
    }

    if ( flags & SYSCALL_NUM )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }
        
        s_out << get_syscall();

        needs_sep = true;
    }

    if ( flags & SYSCALL_ARGS )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_syscall_args();
    }
}
