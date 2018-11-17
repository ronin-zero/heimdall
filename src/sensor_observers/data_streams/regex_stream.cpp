/*
 *  File Name : regex_stream.cpp
 *  
 *  Creation Date : 11-17-2018
 *
 *  Last Modified : Sat 17 Nov 2018 05:47:41 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "regex_stream.h"

Regex_Stream::Regex_Stream( uint_fast8_t out_flags, string sep ){

    syscall_regex = std::regex( Linux_Syscall_Constants::FTRACE_REG );

    flags = out_flags;
    separator = sep;

    out = &std::cout;

    needs_delete = false;
}

Regex_Stream::Regex_Stream( ostream & o_stream, uint_fast8_t out_flags, string sep ){

    syscall_regex = std::regex( Linux_Syscall_Constants::FTRACE_REG );

    flags = out_flags;
    separator = sep;

    out = &o_stream;

    needs_delete = false;
}

Regex_Stream::Regex_Stream( string file_name, uint_fast8_t out_flags, string sep ){

    syscall_regex = std::regex( Linux_Syscall_Constants::FTRACE_REG );

    flags = out_flags;
    separator = sep;
    
    out = new std::ofstream ( file_name );

    needs_delete = true;
}

// Destructor

Regex_Stream::~Regex_Stream(){

    if ( needs_delete )
    {
        delete ( out );
    }
}

// CHECK: I think this might be a problem.  I'm not sure why this is a pointer.
// I'm going to try to pass it by value...

void Regex_Stream::process_data ( Data_Record& record ){

    std::string data_record_string = record.raw_string();

    if ( std::regex_match( data_record_string, syscall_matches, syscall_regex ) )
    {
        bool needs_sep = false;

        if ( flags & PROCESS_NAME )
        {
            *out << syscall_matches.str( Linux_Syscall_Constants::TASK_INDEX );

            needs_sep = true;
        }

        if ( flags & PID )
        {
            if ( needs_sep )
            {
                *out << separator;
            }

            *out << syscall_matches.str( Linux_Syscall_Constants::PID_INDEX );

            needs_sep = true;
        }

        if ( flags & CPU )
        {
            if ( needs_sep )
            {
                *out << separator;
            }

            *out << syscall_matches.str( Linux_Syscall_Constants::CPU_INDEX );

            needs_sep = true;
        }

        if ( flags & TRACE_FLAGS )
        {
            if ( needs_sep )
            {
                *out << separator;
            }

            *out << syscall_matches.str( Linux_Syscall_Constants::TRACE_FLAGS_INDEX );

            needs_sep = true;
        }

        if ( flags & TIMESTAMP )
        {
            if ( needs_sep )
            {
                *out << separator;
            }

            *out << syscall_matches.str( Linux_Syscall_Constants::TIMESTAMP_INDEX );

            needs_sep = true;
        }

        if ( flags & SYSCALL_NUM )
        {
            if ( needs_sep )
            {
                *out << separator;
            }
            
            *out << syscall_matches.str( Linux_Syscall_Constants::SYSCALL_INDEX );

            needs_sep = true;
        }

        if ( flags & SYSCALL_ARGS )
        {
            if ( needs_sep )
            {
                *out << separator;
            }

            *out << syscall_matches.str( Linux_Syscall_Constants::ARGS_INDEX );
        }

        *out << '\n';

    }

    /*record.set_flags( flags );
    record.set_separator( separator );

    *out << record << '\n';*/


}

void Regex_Stream::set_flags( uint_fast8_t new_flags ){

    flags = new_flags;
}

uint_fast8_t Regex_Stream::get_flags(){

    return flags;
}

void Regex_Stream::set_separator( string sep ){

    separator = sep;
}

string Regex_Stream::get_separator(){

    return separator;
}
