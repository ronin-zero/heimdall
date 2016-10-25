/*
 *  File Name : linux_syscall_reader.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Tue 25 Oct 2016 03:27:37 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

//#include <sstream>      // Probably not needed...

#include "linux_syscall_reader.h"

Linux_Syscall_Reader * Linux_Syscall_Reader::lsr_instance = NULL;

Linux_Syscall_Reader * Linux_Syscall_Reader::get_instance(){

    if ( !lsr_instance )
    {
        lsr_instance = new Linux_Syscall_Reader();
    }

    return lsr_instance;
}

Linux_Syscall_Reader * Linux_Syscall_Reader::get_instance( uint_fast8_t flags ){

    lsr_instance = get_instance();

    lsr_instance->configure( flags );

    return lsr_instance;
}

// Deconstructor

Linux_Syscall_Reader::~Linux_Syscall_Reader(){

    stop_reading();
    clear_file( FTRACE_DIR + TRACE );   // Theoretically, this will clear trace_pipe
    set_self_filter( false );
    set_enter( false );
    set_exit( false );
}

uint_fast8_t Linux_Syscall_Reader::set_reading( bool on ){

    if ( on )
    {
        return start_reading();
    }
    else
    {
        return stop_reading();
    }
}

uint_fast8_t Linux_Syscall_Reader::start_reading(){

    if ( !is_reading() )
    {
        update_filter();

        trace_pipe_stream.open( FTRACE_DIR + TRACE_PIPE );

        if ( write_to_file ( FTRACE_DIR + TRACING_ON, "1" ) && trace_pipe_stream.is_open() )
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
        trace_pipe_stream.close();

        if( write_to_file ( FTRACE_DIR + TRACING_ON, "0" ) && !trace_pipe_stream.is_open() )
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

uint_fast8_t Linux_Syscall_Reader::set_self_filter( bool filter ){

    bool success = true;

    string filter_content = ( filter ? build_filter() : "0" );

    for ( std::vector<string>::iterator it = filter_files.begin(); success && it != filter_files.end(); ++it )
    {
        success = write_to_file( *it, filter_content );
    }

    if ( success )
    {
        filter ? status |= FILTER_SELF : status &= ~FILTER_SELF;
    }
    else
    {
        std::cerr << "FILTER FAILURE: " << filter_content << " could not be written to at least one filter file." << std::endl;
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
        std::cerr << "Couldn't set sys_exit to " << std::to_string(exit) << std::endl;
    }

    return status;
}

uint_fast8_t Linux_Syscall_Reader::reading_status(){

    return status;
}

bool Linux_Syscall_Reader::is_reading(){

    return ( status & READING_ON );
}

Sensor_Data * Linux_Syscall_Reader::read_syscall(){

    string tmp = "";
    Sensor_Data * data = NULL;

    if ( is_reading() && trace_pipe_stream.is_open() && !trace_pipe_stream.eof() )
    {
        getline( trace_pipe_stream, tmp );
   
        if ( tmp.length() > MIN_LENGTH )
        {
            data = new Sensor_Data( os, data_type, tmp, "" );
        }
    }

    return data; 
}

// This function seems inelegant. 
// Consider refactoring.

bool Linux_Syscall_Reader::read_syscall_data( string & os_label, string & data_label, string & data ){

    string tmp = "";

    if ( is_reading() && trace_pipe_stream.is_open() && !trace_pipe_stream.eof() )
    {
        getline( trace_pipe_stream, tmp );

        if ( tmp.length() > MIN_LENGTH )
        {
            os_label = os;
            data_label = data_type;
            data = tmp;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


uint_fast8_t Linux_Syscall_Reader::configure( uint_fast8_t flags ){

    set_self_filter( flags & FILTER_SELF );
    set_exit( flags & SYS_EXIT );
    set_enter( flags & SYS_ENTER );
    set_reading( flags & READING_ON );

    return status;
}

// This should be called whenever a thread is created.
// The logic for whether or not a filter should actually be applied
// is handled by the set_self_filter function.  It's of no concern
// to the calling context whether it needs to be set.  Just call
// the function.

void Linux_Syscall_Reader::update_filter(){

    set_self_filter( FILTER_SELF & status );
}

// Protected methods

Linux_Syscall_Reader::Linux_Syscall_Reader( uint_fast8_t flags ){

    status = 0x00;
    clear_file( FTRACE_DIR + TRACE ); // This should clear the trace file and thus trace_pipe.
    status = configure( flags );
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

void Linux_Syscall_Reader::clear_file( string file_name ){

    // This just opens a file for writing (NOT append)
    // and then closes it to clear its contents.
    
    std::cout << "Clearing contents of " << file_name << "." << std::endl;
    
    std::ofstream tmp_file ( file_name, std::ios_base::out );
    tmp_file.close();
}

//  This function gets the PIDs for this program and all the SPIDs (whatever those are)
//  for the threads created by main.  It returns a string formatted such that it will
//  cause ftrace to ignore the system calls made by this program and its threads
//  when the FILTER_SELF flag is set.
//
//  I get these PIDs by getting calling the command 'ls /proc/<PID>/task' where
//  <PID> is the PID of the calling program.  The contents of task will be directories
//  named with the PIDs of this program and the threads associated with it.
//  (Shoutout: Thanks, Bander)
//  
//  I call the command with popen.

string Linux_Syscall_Reader::build_filter(){

    pid_t pid = getpid();

    string command = "ls /proc/" + std::to_string( pid ) + "/task";
    string mode = "r";

    // I can't believe I have to actually convert these strings with .c_str().
    // I mean, it's [CURRENT_YEAR] for crying out loud!

    FILE* pipe = popen( command.c_str(), mode.c_str() );

    // Check if it's NULL

    if ( !pipe )
    {
       return "0";
    }

    string filter = "common_pid != ";

    // We need to remember to separate, but only if necessary.

    bool multiple_pids = false;

    string tmp_entry = "";

    uint_fast8_t tmp_char = 0;

    //char line[128];

    while ( !feof( pipe ) )
    {
        tmp_char = fgetc ( pipe );

        if ( tmp_char == '\n' )
        {
            if ( multiple_pids )
            {
                filter += " && common_pid != ";
            }

            filter += tmp_entry;

            tmp_entry = "";

            multiple_pids = true;
        }

        else if ( ASCII_Operations::is_num( tmp_char ) )
        {
            tmp_entry += tmp_char;
        }
    }

    std::cout << "Filter is: " << filter << "." << std::endl;

    pclose( pipe );
    return filter;
}
