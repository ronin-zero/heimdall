/*
 *  File Name : main.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Tue 05 Jul 2016 12:14:46 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <iostream>
#include <fstream>
#include <string>

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensor_observers/syscall_logger.h"
#include "sensor_observers/data_streams/output_stream.h"
#include "sensor_observers/data_records/linux/linux_syscall_record.h"

#include "command_line_parser.h"

std::vector<string> opt_flags = { "-n", "-p", "-c", "-f", "-t", "-s", "-a", };

int main( int argc, char** argv ){

    Command_Line_Parser parser( argc, argv );

    std::cout << "Printing arguments." << std::endl << std::endl;
    parser.print_args();

    uint_fast8_t flags = 0x00;
    std::string out_file_name = "trace.log";
    std::string separator=",";

    if (  !parser.contains_option( "--flags=" ) && !parser.contains_any( opt_flags ) )
    {
        flags = TIMESTAMP | PROCESS_NAME | PID | SYSCALL_NUM;
    }

    else
    {
        if ( parser.contains_option( "--flags=" ) )
        {
            uint_fast32_t flags_index = parser.option_index( "--flags=" );

            string flags_arg = parser.arg_at( flags_index );

            flags = parser.get_option_value( flags_arg );
        }
        if ( parser.contains_arg( "-n" ) )
        {
            flags |= PROCESS_NAME;
        }

        if ( parser.contains_arg( "-p" ) )
        {
            flags |= PID;
        }

        if ( parser.contains_arg( "-c" ) )
        {
            flags |= CPU;
        }

        if ( parser.contains_arg( "-f" ) )
        {
            flags |= TRACE_FLAGS;
        }

        if ( parser.contains_arg( "-t" ) )
        {
            flags |= TIMESTAMP;
        }

        if ( parser.contains_arg( "-s" ) )
        {
            flags |= SYSCALL_NUM;
        }

        if ( parser.contains_arg( "-a" ) )
        {
            flags |= SYSCALL_ARGS;
        }
    }

    if ( parser.contains_arg( "-o" ) )// && parser.arg_index( "-o" ) < parser.num_args() - 1 )
    {
        std::cout << "Contains flag -o " << std::endl;
        out_file_name = parser.arg_at( parser.arg_index( "-o" ) + 1 );
    }

    if ( parser.contains_option( "--separator=" ) )
    {
        separator = parser.get_option_string( parser.arg_at ( parser.option_index ("--separator=") ) );
    }
    

    std::cout << "You chose to print to file: " << out_file_name << std::endl;
    std::cout << "You chose to use separator: " << separator << std::endl;
    std::cout << "You set your flags to be : " << (int)flags << std::endl;



    return 0;
}
