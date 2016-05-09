/*
 *  File Name : linux_syscall_reader.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Mon 09 May 2016 06:06:55 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <stdint.h>
#include <string>
#include <unistd.h>

#include "syscall_reader.h"

Linux_Syscall_Reader * Syscall_Reader::get_instance( fast_uint8_t flags=SENSOR_DEFAULT){
    
    if ( !sr_instance ){
        sr_instance = new Linux_Syscall_Reader( flags );
    }

    return sr_instance;
}

Linux_Syscall_Reader::Linux_Syscall_Reader( fast_uint8_t flags=SENSOR_DEFAULT ){

    trace_pipe_stream = ifstream( FTRACE_DIR + TRACE_PIPE, std::ifstream::in );

    if ( flags & SENSING_ON )
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

fast_uint8_t set_exit( bool on ){

    try
    {
        
        ofstream ofs( FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_EXIT_DIR + ENABLE, std::ofstream::out );

        if ( on )
        {
            ofstream << "1";

            status |= SYS_EXIT;
        }
        else
        {
            ofstream << "0";

            status ^= SYS_EXIT;
        }

        ofs.close();
    }

    return status;
}

fast_uint8_t set_enter( bool on ){

    try
    {

        ofstream ofs( FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_ENTER_DIR + ENABLE, std::ofstream::out );

        if ( on )
        {
            ofstream << "1";

            status |= SYS_ENTER;
        }
        else
        {
            ofstream << "0";

            status ^= SYS_EXIT;
        }

        ofs.close();
    }

    return status;
}

fast_uint8_t Linux_Syscall_Reader::set_reading_on( bool on ){
    
    if ( bool )
    {
        return start_reading();
    }
    else
    {
        return stop_reading();
    }
}

fast_uint8_t Syscall_Reader::set_self_filter( bool filter ){

    try
    {        
        ofstream ofs ( FTRACE_DIR + NOTRACE, std::ofstream::open );

        if ( filter ) // The option to filter self is true.
        {
            ofs << this.getpid();

            status |= FILTER_SELF;
        }

        ofs.close();
    }
    catch( ofstream failure e )
    {
        std::cerr << "Filter could not be added to " << FTRACE_DIR << NOTRACE << std::endl;
    }
    
    return status;
}


fast_uint8_t Syscall_Reader::start_reading(){

    if ( !is_reading() )
    {

        try 
        {
            ofstream ofs( FTRACE_DIR + TRACING_ON, std::ofstream::out );

            ofs << "1";
    
            ofs.close();

            status |= SENSING_ON;
        }
        catch( ofstream failure e )
        {
            std::cerr << "File " << FTRACE_DIR << TRACING_ON << " was not written to." << std::endl;
        }
    }
   
    return status;
}

fast_uint8_t Syscall_Reader::stop_reading(){

    if ( is_reading() )
    {

        try 
        {
            ofstream ofs( FTRACE_DIR + TRACING_ON, std::ofstream::out );

            ofs << "1";
    
            ofs.close();

            status ^= SENSING_ON;
        }
        catch( ofstream failure e )
        {
            std::cerr << "File " << FTRACE_DIR << TRACING_ON << " was not written to." << std::endl;
        }
    } 

    return status;
}

fast_uint8_t Syscall_Reader::toggle_reading(){

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

fast_uint8_t Syscall_Reader::reading_status(){

    return status;

}

bool Syscall_Reader::is_reading(){

    return ( status & SENSING_ON == SENSING_ON );
}

Sensor_Data Syscall_Reader::read_syscall(){

    string tmp;

    Sensor_Data data;

    if ( is_reading() )
    {
        getline( trace_pipe_stream, tmp );

        data = ( os, data_type, tmp, "" );
    }
    else
    {
        data = NULL;
    }

    return data; 
}
