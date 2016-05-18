/*
 *  File Name : linux_syscall_reader.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Wed 18 May 2016 02:31:46 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <stdint.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>

#include "sensors/syscall_sensors/syscall_readers/linux_syscall_reader.h"

/*
static const string FTRACE_DIR =        "/sys/kernel/debug/tracing/";
static const string TRACING_ON =        "tracing_on";
static const string RAW_SYSCALLS_DIR =  "raw_syscalls/";
static const string SYS_ENTER_DIR =     "sys_enter/";
static const string SYS_EXIT_DIR =      "sys_exit/";
static const string EVENTS_DIR =        "events/";
static const string TRACE_PIPE =        "trace_pipe";
static const string CURRENT_TRACER =    "current_tracer";
static const string ENABLE =            "enable";
static const string NO_TRACE =          "set_ftrace_notrace";


FTRACE_DIR =        "/sys/kernel/debug/tracing/";
TRACING_ON =        "tracing_on";
RAW_SYSCALLS_DIR =  "raw_syscalls/";
SYS_ENTER_DIR =     "sys_enter/";
SYS_EXIT_DIR =      "sys_exit/";
EVENTS_DIR =        "events/";
TRACE_PIPE =        "trace_pipe";
CURRENT_TRACER =    "current_tracer";
ENABLE =            "enable";
NO_TRACE =          "set_ftrace_notrace";*/

Linux_Syscall_Reader * Linux_Syscall_Reader::get_instance( uint_fast8_t flags ){

    if ( !lsr_instance ){
        lsr_instance = new Linux_Syscall_Reader( flags );
    }

    return lsr_instance;
}

Linux_Syscall_Reader::Linux_Syscall_Reader( uint_fast8_t flags ){

    trace_pipe_stream = std::ifstream( FTRACE_DIR + TRACE_PIPE, std::ifstream::in );

    if ( flags & READING_ON )
    {
        start_reading();
    }
    else
    {
        stop_reading();
    }

    if ( flags & FILTER_SELF )
    {
        set_self_filter( true );
    }

    set_enter( flags & SYS_ENTER );

    set_exit ( flags & SYS_EXIT );
}

uint_fast8_t Linux_Syscall_Reader::set_exit( bool on ){

    // "exit" is what will be written to the file.
    // This ternary operator sets it to 1 if we are setting sys_exit on
    // and sets it to 0 if we are setting sys_exit off.

    uint_fast8_t exit = on ? 1 : 0;

    if ( write_to_file ( FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_EXIT_DIR + ENABLE, std::to_string( exit ) ) )
    {
        // Set the SYS_EXIT bit on status to 1 or 0 depending
        // on whether "on" is true or false (respectively).

        ( on ) ? status |= SYS_EXIT : status &= ~SYS_EXIT;
    }
    else
    {
        std::cerr << "Couln't set sys_exit to " << std::to_string(exit) << std::endl;
    }

    return status;
}

uint_fast8_t Linux_Syscall_Reader::set_enter( bool on ){

    // "enter" is what will be written to the file.
    // This ternary operator sets it to 1 if we are setting sys_enter on
    // and sets it to 0 if we are setting sys_enter off.

    uint_fast8_t enter = on ? 1 : 0;

    if( write_to_file( FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_ENTER_DIR + ENABLE, std::to_string( enter ) ) )
    {
        // Set the SYS_ENTER bit on status to 1 or 0 depending
        // on whether "on" is true or false (respectively).

        ( on ) ? status |= SYS_ENTER : status &= ~SYS_ENTER;
    }
    else
    {
        std::cerr << "Couldn't set sys_enter to " << std::to_string( enter ) << std::endl;
    }

    return status;
}

uint_fast8_t Linux_Syscall_Reader::set_reading_on( bool on ){

    if ( on )
    {
        return start_reading();
    }
    else
    {
        return stop_reading();
    }
}

uint_fast8_t Linux_Syscall_Reader::set_self_filter( bool filter ){

    pid_t self_pid = getpid();

    if ( append_to_file ( FTRACE_DIR + NO_TRACE, std::to_string( self_pid ) ) )
    {
        status |= FILTER_SELF;
    }
    else
    {
        std::cerr << "Filter could not be added to " << FTRACE_DIR << NO_TRACE << std::endl;
        std::cerr << "PID is : " << self_pid << std::endl;
    }

    return status;
}


uint_fast8_t Linux_Syscall_Reader::start_reading(){

    if ( !is_reading() )
    {
        if ( write_to_file ( FTRACE_DIR + TRACING_ON, "1" ) )
        {
            status |= READING_ON;
        }
        else
        {
            std::cerr << "Could not start reading." << std::endl;
        }
    }

    return status;
}

uint_fast8_t Linux_Syscall_Reader::stop_reading(){

    if ( is_reading() )
    {
        if( write_to_file ( FTRACE_DIR + TRACING_ON, "0" ) )
        {
            status &= ~READING_ON;
        }
        else
        {
            std::cerr << "Could not stop reading." << std::endl;
        }
    } 

    return status;
}

uint_fast8_t Linux_Syscall_Reader::toggle_reading(){

    if ( is_reading() )
    {
        status = stop_reading();
    }
    else
    {
        status = start_reading();
    }

    return status;
}

uint_fast8_t Linux_Syscall_Reader::reading_status(){

    return status;

}

bool Linux_Syscall_Reader::is_reading(){

    return ( status & READING_ON ) == READING_ON ;
}

Sensor_Data Linux_Syscall_Reader::read_syscall(){

    string tmp;

    if ( is_reading() )
    {
        getline( trace_pipe_stream, tmp );
    }
    else
    {
        tmp = "";
    }
    
    Sensor_Data data( os, data_type, tmp, "" );
    return data; 
}

bool Linux_Syscall_Reader::write_to_file( string filename, string output ){

    return file_write( filename, output, std::ofstream::out );
}

bool Linux_Syscall_Reader::append_to_file( string filename, string output ){

    return file_write( filename, output, std::ofstream::out | std::ofstream::app );
}

bool Linux_Syscall_Reader::file_write( string filename, string output, std::ios_base::openmode mode ){

    try
    {
        std::ofstream ofs( filename, mode );

        ofs << output << std::endl;

        ofs.close();

    }
    catch( std::ofstream::failure &write_err )
    {

        std::cerr << "\n\n Exception occurred when ";

        if ( std::ofstream::app | mode )
        {
            std::cerr << "appending to file.";
        }
        else
        {
            std::cerr << "writing to file.";
        }        

        std::cerr << "\n\nFilename: "<< filename << "\n\n";
        std::cerr << "Exception information:    \n\n";
        std::cerr << write_err.what() << "\n\n";
        std::cerr.flush();

        return false;
    }

    return true;
}
