/*
 *  File Name : trace_reader.cpp
 *  
 *  Creation Date : 03-01-2017
 *
 *  Last Modified : Wed 01 Mar 2017 10:58:23 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "trace_reader.h"

//syscall_position should be the 0-indexed position of the field containing the syscall.

Trace_Reader::Trace_Reader( std::string file_name, uint_fast8_t separator, size_t syscall_position ) : trace_file() {

    _separator = separator;
    _syscall_position = syscall_position;

    _file_name = file_name;

    sep_after_call = 0;

    line_count = 0;

    valid_file = set_parameters();

    if ( valid_file )
    {
        trace_file.open( file_name );
    }
}

Trace_Reader::~Trace_Reader(){

    trace_file.close();
}

bool Trace_Reader::has_next(){

    return !trace_file.eof()
}

int_fast32_t Trace_Reader::next_syscall(){

    // TODO: make this return a meaningful value here.

    return -1;
}

bool Trace_Reader::set_parameters(){

    trace_file.open( _file_name );

    if ( !trace_file.good() )
    {
        if ( trace_file.open() )
        {
            trace_file.close();
        }

        std::cerr << "ERROR: Problem with the file.  It wasn't good." << std::endl;

        return false;
    }

    std::string test_string;

    if ( getline( trace_file, test_string ) )
    {
        // We don't need to read anything else from this file
        // for the rest of this method.

        trace_file.close();

        if ( _syscall_position == std::string::npos )
        {

            // First check to see if the separator even appeas in the record string by
            // searching from the end of the string (rfind).

            size_t sep_index = test_string.rfind( _separator );

            // If the separator isn't found, it's possible that the line only contains the syscall number
            // so check to see if test_string is a number itself.  Regardless of whether it's a number,
            // return the result of is_number on test_string.

            if ( sep_index == std::string::npos )
            {
                if ( is_number( test_string ) )
                {
                    return true;
                }
                else
                {
                    std::cerr << "ERROR: The line provided is not a valid trace record." << std::endl;
                    std::cerr << "OFFENDING LINE: " << test_string << std::endl;

                    return false;
                }
            }

            // Attempt to pull the last field as delimited by the _separator character

            std::string syscall_portion = test_string.substr( sep_index + 1 );

            // Check if the string in this field is a number as defined in ascii_operations.[cpp/h]
            // Granted, this is not fool-proof; the PID will also be a number.  We will search from
            // the end of the line using rfind.  As written, the only option that may appear after 
            // the system call number is the arguments and those are disabled by default.
            // If both the PID and the system call number are enabled when collecting the trace,
            // the system call number will appear AFTER the PID on the line and will be found
            // first when using rfind.  Theoretically, the only way this could go wrong is if one of
            // the arguments to the system call is a number or if the option to record system
            // call numbers was not enabled when collecting the trace.

            if ( is_number( syscall_portion ) )
            {
                return true;
            }

            // If we've reached this point, we now need to iterate through the fields, starting from the right
            // until we either find a field that looks like a syscall number or we run out of fields,
            // returning true or returning false as appropriate.

            size_t next_index = sep_index;

            if ( sep_index > 0 )
            {
                sep_index = test_string.rfind( _separator, sep_index - 1 );
            }

            while ( sep_index != std::string::npos && sep_index > 0 )
            {
                sep_after_call++;

                syscall_portion = test_string.substr( sep_index + 1, next_index - ( sep_index + 1 ) );

                if ( is_number( syscall_portion ) )
                {
                    // This means we've found it. 
                    // We need to set its position by counting how many separators appear before the field.

                    size_t tmp_syscall_position = 1;

                    while ( sep_index > 0 && test_string.rfind( _separator, sep_index -1 ) != std::string::npos  )
                    {
                        tmp_syscall_position++;

                        sep_index = test_string.rfind( _separator, sep_index - 1 ); 
                    }

                    _syscall_position = tmp_syscall_position;

                    return true;
                }

                sep_index = test_string.rfind( _separator, sep_index - 1 );
            }

            // That's all she wrote, this isn't formatted right.

            std::cerr << "ERROR: The line provided is not a valid trace record." << std::endl;
            std::cerr << "OFFENDING LINE: " << test_string << std::endl;

            return false;
        }
        // The given value for the field position isn't n_pos, so verify that the thing at that position is a number.
        else
        {
        }
    }
    else
    {
        trace_file.close();
        return false;
    }
}

bool Trace_Reader::valid_string( std::string syscall_field ){

    size_t sep_index = 0;
    size_t next_index = line.find( _separator, sep_index + 1 );

    for ( int i = 0; i < _syscall_position && next_index != std::string::npos; i++ )
    {
        sep_index = next_index;

        next_index = test_string.find( _separator, sep_index + 1 );
    }


    std::string syscall_portion = test_string.substr( sep_index + 1 );

    return ( is_num (syscall_portion ) );
}

std::string Trace_Reader::get_syscall_field( std::string line ){

    size_t field_start = 0;

    std::string syscall_portion;

    if ( _syscall_position == std::string::npos )
    {
        field_start = line.rfind( _separator );

        if ( field_start == std::string::npos )
        {
            syscall_portion = line;
        }
        else
        {
            syscall_portion = line.substr( field_start + 1 );
        }
    }
    else
    {
        
    }

}
