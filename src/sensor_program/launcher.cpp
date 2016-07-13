/*
 *  File Name : main.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Wed 13 Jul 2016 05:01:27 PM EDT
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

#include "command_line_utils/command_line_parser.h"
#include "sensor_manager/sensor_manager.h"

std::vector<std::string> opt_flags = { "-n", "-p", "-c", "-f", "-t", "-s", "-a", };

const std::string pipe_name = "/var/run/sensor.pipe";

std::string flag_string( uint_fast8_t flags );
//std::string get_program_name( char* arg );

void start( Command_Line_Parser parser );
void stop( Command_Line_Parser parser );
void status( Command_Line_Parser parser );
bool is_running( std::string program_name );


int main( int argc, char** argv ){

    Command_Line_Parser command_parser( argc, argv );

    if ( command_parser.contains_arg( "-h" ) || command_parser.contains_option( "--help" ) )
    {
        command_parser.print_help();
    }
    else if ( command_parser.check_args() )
    {
        if ( command_parser.contains_arg( "start" ) )
        {
            start( command_parser );
        }
        else if ( command_parser.contains_arg( "status" ) )
        {
            status( command_parser );
        }
        else if ( command_parser.contains_arg( "stop" ) )
        {
            stop( command_parser );
        }
    }

    else
    {
        return -1;
    }

    return 0;
}

std::string flag_string( uint_fast8_t flags ){

    std::string string_flag = "";

    uint_fast32_t flag_count = 0;

    if ( flags & PROCESS_NAME )
    {
        string_flag += "PROCESS_NAME";

        flag_count++;
    }

    if ( flags & PID )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "PID";

        flag_count++;
    }

    if ( flags & CPU )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "CPU";

        flag_count++;
    }

    if ( flags & TRACE_FLAGS )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "TRACE_FLAGS";

        flag_count++;
    }

    if ( flags & TIMESTAMP )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "TIMESTAMP";

        flag_count++;
    }

    if ( flags & SYSCALL_NUM )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "SYSCALL_NUM";

        flag_count++;
    }

    if ( flags & SYSCALL_ARGS )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "SYSCALL_ARGS";

        flag_count++;
    }

    return string_flag;
}

std::string get_program_name( char* arg ){

    std::string argument( arg );

    std::string leading_slash = "./";

    size_t slash_pos = argument.find( leading_slash );

    if ( slash_pos == std::string::npos )
    {
        return argument;
    }
    else
    {
        return argument.substr( slash_pos + leading_slash.length() );
    }
}

void start ( Command_Line_Parser parser ){

    std::string prog_name = parser.get_program_name();

    if ( is_running ( prog_name ) )
    {

        pid_t pid = Daemonizer::get_daemon_pid( prog_name );

        std::cout << prog_name << " is already running with PID " << pid << std::endl;

    }
    else
    {
        mkfifo ( pipe_name.c_str(), 0666 );

        uint_fast8_t flags = 0x00;
        std::string out_file_name = "trace.log";
        std::string separator=",";
        bool run_daemon = true;

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

        if ( parser.contains_option( "--daemon=" ) )
        {
            std::string daemon_option = parser.get_option_string( parser.arg_at ( parser.option_index ( "--daemon=" ) ) );

            // TODO: This should be cleaned up to do a better case-insensitive comparison.
            // In the interest of time, I just wrote this quickly.

            if ( daemon_option == "0" || daemon_option == "OFF" || daemon_option == "off" 
                    || daemon_option == "Off" || daemon_option == "oFf" || daemon_option == "ofF"
                    || daemon_option == "oFF" || daemon_option == "OFf" || daemon_option == "OfF" )
            {
                run_daemon = false;
            }
            else if ( daemon_option == "1" || daemon_option == "ON" || daemon_option == "on" || daemon_option == "oN" || daemon_option == "On" )
            {
                run_daemon = true;
            }
            else
            {
                std::cerr << "WARNING: Argument " << daemon_option << " passed to option --daemon is invalid." << std::endl;
                std::cerr << "Option/argument must be of the form: --daemon=[ON|1] to run as daemon or --daemon=[OFF|0] to run" << std::endl;
                std::cerr << "as a normal application (see help: run with -h or --help)." << std::endl;
            }
        }


        std::cout << "You chose to print to file: " << out_file_name << std::endl;
        std::cout << "You chose to use separator: " << separator << std::endl;
        std::cout << "You set your flags to be : " << (int)flags << " -- " << flag_string( flags ) << std::endl;

        if ( run_daemon )
        {
            std::cout << "Sensor will be run as a daemon." << std::endl;
        }
        else
        {
            std::cout << "Daemonization disabled.  Sensor will be run as a regular application." << std::endl;
        }

        Sensor_Manager manager( flags, out_file_name, separator, prog_name );

        manager.run_sensor( run_daemon );
    }
}

void status ( Command_Line_Parser parser ){

    if ( is_running( parser.get_program_name() ) )
    {
        std::cout << "The sensor is running with PID " << Daemonizer::get_daemon_pid( parser.get_program_name() ) << std::endl;
    }
    else
    {
        std::cout << "The sensor is not running." << std::endl;
    }
}

void stop ( Command_Line_Parser parser ){

    if ( !is_running( parser.get_program_name() ) )
    {
        std::cout << "The sensor is not running." << std::endl;
    }
    else
    {
        int32_t fd;

        fd = open( pipe_name.c_str(), O_WRONLY );

        std::string stop = "STOP";

        const char* c_stop = stop.c_str();

        if ( !write( fd, c_stop, sizeof( c_stop ) ) )
        {
            std::cerr << "ERROR: Couldn't write to " << pipe_name << " -- perhaps you need root/sudo?" << std::endl;
        }
        else
        {
            close( fd );

            unlink( pipe_name.c_str() );

            Daemonizer::remove_daemon_pid( parser.get_program_name() );
        }
    }
}

bool is_running( std::string program_name ){

    pid_t pid = Daemonizer::get_daemon_pid( program_name );

    return pid > 0;
}
