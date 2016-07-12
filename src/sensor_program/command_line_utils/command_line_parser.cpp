/*
 *  File Name : command_line_parser.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Mon 11 Jul 2016 08:01:04 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "command_line_parser.h"

Command_Line_Parser::Command_Line_Parser( int argc, char** argv ){

    if ( argc <= 0 )
    {
        std::cerr << "ERROR: There are no arguments.  Please check your main program." << std::endl;
    }
    else
    {
        program_name = parse_program_name( argv[0] );

        for ( int i = 1; i < argc; i++ ){

            // Convert each char* in argv into a std::string...
            std::string tmp( argv[i] );

            // Store the std::string in a vector.
            arguments.push_back( tmp );
        }
    }
}

bool Command_Line_Parser::contains_arg( std::string arg ){

    return ( arg_index( arg ) >= 0 );
}

bool Command_Line_Parser::contains_option( std::string opt ){

    return ( option_index( opt ) >= 0  );
}

bool Command_Line_Parser::contains_any( std::vector<std::string> args ){

    for ( uint_fast32_t i = 0; i < args.size(); i++ )
    {
        if ( contains_arg( args[i] ) )
        {
            return true;
        }
    }

    return false;
}

std::string Command_Line_Parser::get_program_name(){

    return program_name;
}

int_fast64_t Command_Line_Parser::arg_index( std::string arg, uint_fast32_t pos ){

    int_fast64_t index = -1;

    for ( uint_fast32_t i = pos; i < arguments.size(); i++ )
    {
        if ( arguments[i] == arg )
        {
            return i;
        }
    }

    return index;
}

int_fast64_t Command_Line_Parser::option_index( std::string opt, uint_fast32_t pos ){

    int_fast64_t index = -1;

    for ( uint_fast32_t i = pos; i < arguments.size(); i++ )
    {
        if ( arguments[i].find( opt ) != std::string::npos )
        {
            return i;
        }
    }

    return index;
}

int_fast32_t Command_Line_Parser::get_option_value( std::string arg ){

    // TODO: convert the value passed to this option to an unsigned int.
    // Accept both hex and base 10 values.

    int_fast32_t opt_val = -1;

    std::string opt_arg = get_option_string( arg );

    if ( ASCII_Operations::is_hex_byte( opt_arg ) )
    {
        opt_val = ASCII_Operations::hex_byte_val( opt_arg );
    }
    else if ( ASCII_Operations::is_number( opt_arg ) )
    {
        opt_val = ASCII_Operations::to_int( opt_arg );
    }

    return opt_val;
}

std::string Command_Line_Parser::get_option_string( std::string arg ){

    // TODO: Parse the argument to get the value of the option after
    // the '=' character.

    int_fast32_t opt_start  = arg.find("=") + 1;

    std::string raw_opt_string = arg.substr( opt_start );

    std::string opt_string = replace_tab_char( raw_opt_string ); 

    if ( opt_string.length() < 1 )
    {
        std::cerr << "FAILURE - Invalid argument to option: " << arg.substr( 0, opt_start - 1 ) << std::endl;
        std::cerr << "Full argument was: " << arg << std::endl;
        std::cerr << "opt_string was: " << opt_string << std::endl;
    }

    return opt_string;
}

/*
   void Command_Line_Parser::parse_args( uint_fast32_t argc, char** argv ){

   std::cout << "Number of arguments: " << argc << std::endl;

   std::cout << "They are as follows: " << std::endl;

   for ( uint_fast32_t i = 0; i < argc; i++ )
   {
   std::cout << "arg" << i << ": " << argv[i] << "  ";
   }

   std::cout << std::endl;
   }*/

void Command_Line_Parser::print_help(){

    std::cout << "HEIMDALL syscall-sensor" << std::endl << std::endl;

    std::cout << "v 1.0.0" << std::endl << std::endl;

    std::cout << "DESCRIPTION" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "This program traces raw system calls and logs them to a specified output file." << std::endl;
    std::cout << std::string( 8, ' ' ) << "See OPTIONS for what data can be traced and logged." << std::endl;
    std::cout << std::string( 8, ' ' ) << "See COMMANDS for what actions can be taken." << std::endl;
    std::cout << std::string( 8, ' ' ) << "See DEFAULTS for the behavior when no options are set." << std::endl << std::endl;

    std::cout << "USAGE" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "syscall-sensor [COMMAND] [OPTION]" << std::endl << std::endl;

    std::cout << "COMMANDS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setfill('.') << std::setw(20) << std::left << "start" ;
    std::cout << "Starts tracing accorinding to the given OPTIONS." << std::endl;
    std::cout << std::string( 28, ' ' ) << "Begins reading system call data from the trace pipe and logging it to the specified output file." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setfill('.') << std::setw(20) << std::left << "stop" ;
    std::cout << "Stops the sensor (if it is running)." << std::endl;
    std::cout << std::string( 28, ' ' ) << "Cleanly closes the open files, resets the trace pipe, clears filters, and closes the log file." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setfill('.') << std::setw(20) << std::left << "status";
    std::cout << "Shows the status of the sensor." << std::endl;
    std::cout << std::string( 28, ' ' ) << "Status information includes whether the sensor is running and what data is being recorded." << std::endl << std::endl;


    std::cout << "OPTIONS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "A system call record contains various information about a process, not all of which may be useful" << std::endl;
    std::cout << std::string( 8, ' ' ) << "in every situation.  This portion of the manual is broken into five sections: " << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "1. OUTPUT FILE - How to specify the file to which output should be logged." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "2. SYSTEM CALLS - An explanation of what data is contained in a system call record caputured in" << std::endl;
    std::cout << std::string( 11, ' ' ) << "ftrace with an example of such a record with its fields labeled." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "3. DATA OPTIONS - How to configure the sensor in order to specify what data fields of a system" << std::endl; 
    std::cout << std::string( 11, ' ' ) << "call record should be included in the output log file as well as how to specify" << std::endl;
    std::cout << std::string( 11, ' ' ) << "a character or std::string to use as a field separator in the logged output (3.1).  The section" << std::endl;
    std::cout << std::string( 11, ' ' ) << "concludes with a table of data fields and their corresponding flags (3.2)." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "4. RUNNING OPTIONS - The program can be run in the background as a daemon-like service." << std::endl;
    std::cout << std::string( 11, ' ' ) << "Additionally, the sensor can be configured to either include or ignore system call records" << std::endl;
    std::cout << std::string( 11, ' ' ) << "generated by processes related to the sensor itself." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "5. DEFAULT CONFIGURATION - The configuration of the sensor if it is run with no specified" << std::endl;
    std::cout << std::string( 11, ' ' ) << "options at runtime with an example of what a line in the log file would look like with this" << std::endl;
    std::cout << std::string( 11, ' ' ) << "configuration specified." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "1. OUTPUT FILE" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-o output_file" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Writes the sensor data to the file specified by \'output_file\'.  The path is relative from wherever" << std::endl;
    std::cout << std::string( 11, ' ' ) << "the command to run the sensor was called.  The filename cannot contain white space." << std::endl;
    std::cout << std::string( 11, ' ' ) << "WARNING: This program should be considered a rough beta.  It strongly recommended that you avoid using" << std::endl;
    std::cout << std::string( 11, ' ' ) << "meta characters or characters with special significance to the terminal.  The input is not guaranteed to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "sanitized and any such behavior is undefined AT BEST.  The user assumes all risk for using this program." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "2. SYSTEM CALLS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "A record generated by ftrace looks like the third line of this labeled example:" << std::endl << std::endl;

    std::cout << std::string( 15, ' ' ) << "TASK-PID   CPU# TFLAGS    TIMESTAMP  FUNCTION  SYSCALL#          SYSCALLARGS" << std::endl;
    std::cout << std::string( 15, ' ' ) << "   | |       |   ||||       |         |             |                  |"      << std::endl;
    std::cout << std::string( 15, ' ' ) << "bash-15509 [002] .... 112170.687012: sys_enter: NR 16 (0, 5403, 7ffd0c9241f0, 0, 0, 700600)" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "A brief explanation of each field follows." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "TASK" << std::endl;
    std::cout << std::string( 11, ' ' ) << "This is the name of the process that generated the record.  In the example, it is \'bash\'." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "PID" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Stands for \"Process ID.\"  It is a number associated with a process that serves as a unique, uniform" << std::endl;
    std::cout << std::string( 11, ' ' ) << "identifier.  Every running process has a PID." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "CPU#" << std::endl;
    std::cout << std::string( 11, ' ' ) << "CPU stands for \"Central Processing Unit.\"  On systems with multiple CPUs and/or multiple CPU cores," << std::endl;
    std::cout << std::string( 11, ' ' ) << "process run simultaneously may be run on separate processors/cores.  The CPU# indicates which CPU/core" << std::endl;
    std::cout << std::string( 11, ' ' ) << "is running that process." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "TFLAGS" << std::endl;
    std::cout << std::string( 11, ' ' ) << "These are the flags representing certain low-level system information for a process.  There are four flags" << std::endl;
    std::cout << std::string( 11, ' ' ) << "and they are as follows: \'irqs-off,\' \'needs-resched,\' \'hardirq/softirq,\' and \'preempt-depth\'" << std::endl;
    std::cout << std::string( 11, ' ' ) << "(respectively).  Normally, all four will be shown as \'....\' and a user is unlikely to need this field" << std::endl;
    std::cout << std::string( 11, ' ' ) << "in a trace.  It is still given as an option for the sake of completeness.  For more information, please see" << std::endl;
    std::cout << std::string( 11, ' ' ) << "the documentation for ftrace as an in-depth explanation of these fields is beyond the scope of this program's" << std::endl;
    std::cout << std::string( 11, ' ' ) << "concerns and responsiblities." << std::endl << std::endl;


    std::cout << std::string( 8, ' ' ) << "TIMESTAMP" << std::endl;
    std::cout << std::string( 11, ' ' ) << "This is the time (in miliseconds) at which the function that generated this record was entered." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "FUNCTION" << std::endl;
    std::cout << std::string( 11, ' ' ) << "ftrace can be configured to trace specific functions and system calls.  This program traces the \'sys_enter\'" << std::endl;
    std::cout << std::string( 11, ' ' ) << "event which occurs when any system call is made.  This field should always read \'sys_enter.\'  As such, this" << std::endl;
    std::cout << std::string( 11, ' ' ) << "field is omitted by the logged data generated by this program (the field \'NR\' that follows is also constant" << std::endl;
    std::cout << std::string( 11, ' ' ) << "for all records observed by this program and is similarly omitted as it provides no useful information in this" << std::endl;
    std::cout << std::string( 11, ' ' ) << "context).  Information regarding which system call was issued is given by SYSCALL#, the following field." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "SYSCALL#" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Every system call has an associated number.  The number of a given system call, the number of total system" << std::endl;
    std::cout << std::string( 11, ' ' ) << "calls available to a system, and even the range of values for system call numbers vary greatly depending on" << std::endl;
    std::cout << std::string( 11, ' ' ) << "the system architecture and operating system.  For instance, on a 64-bit Linux Mips machine, system call number" << std::endl;
    std::cout << std::string( 11, ' ' ) << "values range from 5000 to 5305 (inclusive) and there are 305 system calls in the instruction set (see: Note 1), but" << std::endl;
    std::cout << std::string( 11, ' ' ) << "on an x86 Linux system, system call number values range from 0 to 349 and there are 346 system calls in the" << std::endl; 
    std::cout << std::string( 11, ' ' ) << "instruction set.  The example in this section was collected on a system running x86-64 Linux where the number" << std::endl;
    std::cout << std::string( 11, ' ' ) << "of the system call made by bash is \'16,\' which corresponds to system call \'ioctl\' (see \'man ioctl\' for" << std::endl;
    std::cout << std::string( 11, ' ' ) << "information on this system call)." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "SYSCALLARGS" << std::endl;
    std::cout << std::string( 11, ' ' ) << "These are the arguments that were given to the system call made by the process that generated this record." << std::endl;
    std::cout << std::string( 11, ' ' ) << "They may vary in data type (numbers, std::strings, etc.), values, and even number of arguments.  Normally, this" << std::endl;
    std::cout << std::string( 11, ' ' ) << "field can be safely omitted from a trace without losing pertinent information, but it may be optionally enabled" << std::endl;
    std::cout << std::string( 11, ' ' ) << "for the sake of completeness or in case these data can be used for a separate metric in the future." << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "Note 1: Not all of the numbers in the range may have an associated system call or the function corresponding to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "a number may not be implemented.  If the range of system call numbers for an instruction set is \'n\' to \'m,\'" << std::endl;
    std::cout << std::string( 11, ' ' ) << "inclusive, there are AT MOST ( m - n ) + 1 system calls in that set." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "3. DATA OPTIONS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "3.1 Data Record Fields and Field Separator" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "A system call record generated by ftrace contains a lot of information (see: Section 2. SYSTEM CALLS for more" << std::endl;
    std::cout << std::string( 8, ' ' ) << "information and an example), not all of which may be desirable in a generated log.  There are fields that are" << std::endl;
    std::cout << std::string( 8, ' ' ) << "included in a generated log by default (see: Section 5. DEFAULT OPTIONS).  These fields (listed below) each have" << std::endl;
    std::cout << std::string( 8, ' ' ) << "an associated \"flag\" value.  These fields can be configured to be omitted or included in the log either by" << std::endl;
    std::cout << std::string( 8, ' ' ) << "individually giving the option for each separately or by passing the sum (boolean \'or\') of the flags corresponding" << std::endl;
    std::cout << std::string( 8, ' ' ) << "to the desired fields to the \'--flags=\' option (either as a two-digit hex value or a decimal number ranging from" << std::endl;
    std::cout << std::string( 8, ' ' ) << "0 to 255, inclusive).  The two methods can be mixed; the result will just be the inclusion of all the fields" << std::endl;
    std::cout << std::string( 8, ' ' ) << "specified by either method.  A table of data fields and their corresponding flag values and option flags is included" << std::endl;
    std::cout << std::string( 8, ' ' ) << "at the end of this subsection." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "--flags=FLAGS" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Specify what fields should be included in the output.  The value of FLAGS is interpreted as the number representing" << std::endl;
    std::cout << std::string( 11, ' ' ) << "the values of the flags corresponding to the fields you wish to include in your output \'or\'d\' together.  The" << std::endl;
    std::cout << std::string( 11, ' ' ) << "value of FLAGS can be given either as a decimal integer between 0 and 255, inclusive, or as the hexidecimal" << std::endl;
    std::cout << std::string( 11, ' ' ) << "representation of the same value.  Hexidecimal values must be in the range 0x00 to 0xFF, inclusive and MUST be" << std::endl;
    std::cout << std::string( 11, ' ' ) << "preceeded by 0x as shown." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-n" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the name of the process (TASK in section 2) in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-p" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the process ID of the process (PID in section 2) in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-c" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the CPU number of the process (CPU# in section 2) in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-f" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the field with information about IRQs and scheduling for the process (TFLAGS in section 2)" << std::endl;
    std::cout << std::string( 11, ' ' ) << "in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-t" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the timestamp field for the process (TIMESTAMP in section 2) in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-s" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the number of the system call made by the process (SYSCALL# in section 2) in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-a" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Include the arguments to the system call made by the process (SYSCALLARGS in section 2) in the log." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "--separator=SEP" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Specify how the fields should be separated in the output log.  SEP is interpreted as a string." << std::endl;
    std::cout << std::string( 11, ' ' ) << "WARNING: As this program should be considered a rough beta, the functionality of this option is still" << std::endl;
    std::cout << std::string( 11, ' ' ) << "somewhat limited.  It SHOULD support meta characters such as \\t (for tab) and it SHOULD be able to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "support values for SEP that include whitespace so long as they are within double quotes (\")." << std::endl;
    std::cout << std::string( 11, ' ' ) << "Still, this is an experimental program that is still in development, so no promises are made and no" << std::endl;
    std::cout << std::string( 11, ' ' ) << "guarantees are given." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "--daemon=[ON|OFF|0|1]" << std::endl;
    std::cout << std::string( 11, ' ' ) << "Choose whether to run the sensor as a background, daemon-like process.  Options ON and 1 will set" << std::endl;
    std::cout << std::string( 11, ' ' ) << "the sensor to launch as a daemonized, background process.  Options OFF and 0 will set the sensor to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "run in the forground like a regular application.  Options OFF and ON should be case-insensitive, but" << std::endl;
    std::cout << std::string( 11, ' ' ) << "entering them in all caps will work." << std::endl << std::endl;
    std::cout << std::string( 11, ' ' ) << "The sensor program runs as a daemon by default." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "3.2 Table of Data Field Options" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "Field Name";
    std::cout << std::setw(20) << std::setfill(' ') << std::left << "Flag Value (Decimal)";
    std::cout << std::setw(20) << std::setfill(' ') << std::left << "Flag Value (Hex)";
    std::cout << std::setw(20) << std::setfill(' ') << std::right << "Option Flag" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::string(80, '=') << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "Process Name";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "128";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x80";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-n"; 

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "PID";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "64";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x40";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-p";

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "CPU";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "32";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x20";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-c"; 

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "Trace Flags";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "16";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x10";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-f";

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "Timestamp";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "8";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x08";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-t"; 

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "System Call Number";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "4";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x04";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-s"; 

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setw(20) << std::setfill('.') << std::left << "System Call Args";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "2";
    std::cout << std::setw(20) << std::setfill('.') << std::left << "0x02";
    std::cout << std::setw(20) << std::setfill('.') << std::right << "-a";

    std::cout << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "4. RUNNING OPTIONS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "This section is a work in progress.  It has no supported options at present.  In the future, I hope to" << std::endl;
    std::cout << std::string( 8, ' ' ) << "include options that will allow the user to select whether to run the program as a background daemon-like" << std::endl;
    std::cout << std::string( 8, ' ' ) << "service or as a foreground program as well as options to specify how long the sensor should run." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "5. DEFAULT CONFIGURATION" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "When made to run without specifying any arguments, the program will run as a daemon-like background process." << std::endl;
    std::cout << std::string( 8, ' ' ) << "The fields for the name of the process, PID, timestamp, and system call number will be logged as a single line" << std::endl;
    std::cout << std::string( 8, ' ' ) << "separated by a comma (\',\') as the default separator (SEP value).  This is effectively a CSV.  Data will be" << std::endl;
    std::cout << std::string( 8, ' ' ) << "to a file named \"trace.log\" in the directory from which the command to execute the program was given. It is" << std::endl;
    std::cout << std::string( 8, ' ' ) << "equivalent executing any of the following commands:" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "syscall-sensor start" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "syscall-sensor start -n -p -t -s --separator=, -o trace.log" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "syscall-sensor start --flags=0xCC --separator=, -o trace.log" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "syscall-sensor start --flags=204 --separator=, -o trace.log" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "If the record in the example in section 2 appeared in a trace during execution of this program with the default" << std::endl;
    std::cout << std::string( 8, ' ' ) << "settings (or with the explicit options above), its corresponding record in the log would appear as follows:" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "bash,15509,112170.687012,16" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "(without the leading whitespace)" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "Please report bugs to:" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "ronin-zero@github.com" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "The project's main repository can be found at:" << std::endl << std::endl;
    std::cout << std::string( 11, ' ' ) << "https://github.com/ronin-zero/heimdall" << std::endl << std::endl;

    std::cout << "Written by: 浪人ー無, 2016" << std::endl << std::endl;
}

void Command_Line_Parser::print_usage(){

    std::cout << std::endl << "USAGE:" << std::endl;
    std::cout << std::string( 8, ' ' ) << "syscall-sensor COMMAND [OPTIONS]" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "COMMANDs" << std::endl << std::endl;
    std::cout << std::string( 11, ' ' ) << std::setw(40) << std::setfill(' ') << std::left << "start" << "start the sensor with [OPTIONS]" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(40) << std::setfill(' ') << std::left << "stop" << "stop the sensor" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(40) << std::setfill(' ') << std::left << "status" << "check sensor status (NOT IMPLEMENTED)" << std::endl;
    std::cout << std::endl;

    std::cout << std::string( 8, ' ' ) << "OPTIONS" << std::endl << std::endl;
    
    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "--flags=FLAGS";
    std::cout << "run sensor with flags [FLAGS].  Accepts decimal values and hex of the form 0xXX (default: 204, or 0xCC)" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "--separator=SEP";
    std::cout << "separate fields in the log with [SEP] (default: ,)" << std::endl;
    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "--daemon=[ON|OFF]";
    std::cout << "run the sensor as a daemon-like background process (ON; the default) or as a regular application (OFF). (ADVANCED)" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-o output_file";
    std::cout << "log output to output_file (default: trace.log)" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-n";
    std::cout << "Include the process\' name in the logged information" << std::endl;
    
    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-p";
    std::cout << "Include the process\' PID in the logged information" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-c";
    std::cout << "Include the process\' CPU number in the logged information" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-f";
    std::cout << "Include the process\' irq information in the log (see help)" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-t";
    std::cout << "Include the timestamp of the syscall record in the logged information" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-s";
    std::cout << "Include the syscall number in the logged information" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-a";
    std::cout << "Include the syscall's arguments in the logged information" << std::endl;

    std::cout << std::endl;

    std::cout << "Run with flag -h to view the help documentation (it is recommended that you pipe it to a pager" << std::endl;
    std::cout << "such as \'less\' for ease of reading.  Example: \'syscall-sensor -h | less\')." << std::endl;
}

// This function is to make sure that pairs of quotes are balanced.
// The user can specify any char at runtime, but the intention is
// to check for quotes.
bool Command_Line_Parser::check_balance( std::string input, int_fast8_t q ){

    int_fast32_t input_length = input.length();

    // If it's only one character long, it can't be a quotation mark.
    // If it's 0 characters long, it's not worth checking the rest.

    if ( input_length > 2 )
    {

        int_fast32_t first_index = input.find( q );
        int_fast32_t second_index = input.rfind( q );

        return  ( first_index == 0 && second_index == input_length - 1 );
    }

    return false;
}

std::string Command_Line_Parser::strip_endpoints( std::string input ){

    return input.substr( 1, input.length() - 2 );
}

std::string Command_Line_Parser::sanitize_input( std::string input ){

    std::string sanitized = "";

    if ( check_balance( input, '"' ) )
    {
        sanitized = strip_endpoints( input );
    }
/*    else if ( check_balance( input, '\'' ) )
    {
        sanitized = strip_endpoints( input );
        
        if ( sanitized.length() > 2 )
        {
            sanitized = "";
        }
        
    }*/
    else
    {
        sanitized = input;
    }
    std::cout << "Sanitized string is " << sanitized << std::endl;
    return sanitized;
}

std::string Command_Line_Parser::replace_tab_char( std::string input ){

    std::string output = "";

    std::string tab_char = "\\t";

    uint_fast32_t tab_index = input.find( "\\t" );

    if ( tab_index == std::string::npos )
    {
        output = input;
    }
    else
    {
        output += input.substr( 0, tab_index );
        output += '\t';
        output += replace_tab_char( input.substr( tab_index + 2 ) );
    }

    return output;
}

std::string Command_Line_Parser::parse_program_name( char* arg ){

    std::string name( arg );

    std::string dot_slash = "./";

    std::size_t slash_pos = name.find( dot_slash );

    if ( slash_pos == std::string::npos )
    {
        return name;
    }

    else
    {
        return name.substr( slash_pos + dot_slash.length() );
    }
}

void Command_Line_Parser::print_args(){

    for ( uint_fast32_t i = 0; i < arguments.size(); i++ )
    {
        std::cout << "Arg #" << i << ": " << arguments[i] << std::endl;
    }
}

bool Command_Line_Parser::check_args(){

    uint_fast32_t commands = 0;
    uint_fast32_t errors = 0;

    bool valid_args = true;

    std::vector<uint_fast32_t> bad_args;

    for ( uint_fast32_t i = 0; i < arguments.size(); i++ )
    {
        if ( valid_command( arguments[i] ) )
        {
            commands++;
        }
        else if ( !( valid_option( arguments[i] ) || valid_arg( arguments[i] ) ) )
        {
            if ( i > 0 && arguments[ i -1 ] != "-o" )
            {
                errors++;
                bad_args.push_back(i);
            }
        }
    }

    if ( commands != 1 )
    {
        errors++;
    }

    if ( errors > 0 )
    {
        valid_args = false;

        std::cerr << "ERROR: " << errors << " errors in arguments." << std::endl;
        
        print_malformed_args( bad_args, commands );
    }

    return valid_args;
}

bool Command_Line_Parser::valid_command( std::string input ){

    for ( uint_fast32_t i = 0; i < commands.size(); i++ )
    {
        if ( input == commands[i] )
        {
            return true;
        }
    }

    return false;
}

bool Command_Line_Parser::valid_option( std::string input ){

    for ( uint_fast32_t i = 0; i < opt_flags.size(); i++ )
    {
        if ( input.find( opt_flags[i] ) == 0 && opt_flags[i].length() < input.length() )
        {
            return true;
        }
    }

    return false;
}

bool Command_Line_Parser::valid_arg( std::string input ) {

    for ( uint_fast32_t i = 0; i < arg_flags.size(); i++  )
    {
        if ( input == arg_flags[i] )
        {
            return true;
        }
    }

    return false;
}

void Command_Line_Parser::print_malformed_args( std::vector<uint_fast32_t> malformed_args, uint_fast32_t num_cmds ){

    if ( num_cmds == 0 )
    {
        std::cerr << "ERROR: No commands (start/status/stop) given." << std::endl;
    }
    
    if ( num_cmds > 1 )
    {
        std::cerr << "ERROR: Too many commands. Only one start/status/stop command may be given." << std::endl;
    }

    for ( uint_fast32_t i = 0; i < malformed_args.size(); i++ )
    {
        std::string tmp_arg = arguments[ malformed_args[i] ];

        std::cerr << "ERROR - Bad argument #" << i << " as argument " << malformed_args[i] << std::endl;

        if ( tmp_arg.length() <= 2 )
        {
            std::cerr << "Unrecognized argument \"" << tmp_arg << "\"" << std::endl;
        }
        else if ( tmp_arg.find( "--separator=" ) != std::string::npos )
        {
            std::string option = tmp_arg.substr( tmp_arg.find('=') + 1 );

            if ( option.length() < 1 )
            {
                std::cerr << "--separator requires an argument." << std::endl;
            }
            else
            {
                std::cerr << option << " is not a valid argument for option --separator" << std::endl;
            }
        }
        else if ( tmp_arg.find( "--flags=" ) != std::string::npos )
        {
            std::string option = tmp_arg.substr( tmp_arg.find('=') + 1 );

            if ( option.length() < 1 || ( !ASCII_Operations::is_hex_byte( option ) && !ASCII_Operations::is_number( option ) ) )
            {
                std::cerr << "--flags requires an argument of either a decimal or hexidecimal number between 0 and 255 (inclusive)" << std::endl;
            }
            else
            {
                std::cerr << "Argument \"" << option << "\" to option --flags is malformed." << std::endl;
            }
        }
        else
        {
            std::cerr << "Unspecified problem with argument " << tmp_arg << std::endl;
        }
    }

    print_usage();
}
