/*
 *  File Name : linux_syscall_reader.h
 *  
 *  Creation Date : 05-09-2017
 *
 *  Last Modified : Sun 07 Oct 2018 03:29:35 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <vector>
#include <unistd.h>

#include "syscall_reader.h"
#include "utils/ascii_operations.h"

static const std::string FTRACE_DIR =        "/sys/kernel/debug/tracing/";
static const std::string TRACE =             "trace";
static const std::string TRACING_ON =        "tracing_on";
static const std::string RAW_SYSCALLS_DIR =  "raw_syscalls/";
static const std::string SYS_ENTER_DIR =     "sys_enter/";
static const std::string SYS_EXIT_DIR =      "sys_exit/";
static const std::string EVENTS_DIR =        "events/";
static const std::string TRACE_PIPE =        "trace_pipe";
static const std::string CURRENT_TRACER =    "current_tracer";
static const std::string ENABLE =            "enable";
static const std::string NO_TRACE =          "set_ftrace_notrace";
static const std::string FILTER  =           "filter";
static const std::string FILTER_SYS_ENTER =  FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_ENTER_DIR + FILTER;
static const std::string FILTER_SYS_EXIT =   FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_EXIT_DIR + FILTER;

static const uint_fast8_t MIN_LENGTH = 50;  // This is the FUNCTION_INDEX (see: linux_syscall_record.h lines 65 and 81).
                                            // Sometimes a line like "CPU:13 [LOST 28120 EVENTS]" will appear in the trace pipe.
                                            // Obviously, we can't parse this like a regular system call record.  A valid system call record
                                            // will have a function in it, so there will be something at index 50 and beyond, so if it's less
                                            // than 50 characters long, just ignore it.

class Linux_Syscall_Reader:public Syscall_Reader{

    public:

        static Linux_Syscall_Reader * get_instance();
        static Linux_Syscall_Reader * get_instance( uint_fast8_t flags );
        ~Linux_Syscall_Reader();

        uint_fast8_t set_reading( bool on );

        // CHECK: Shawn feels these toggle methods
        // are unnecessary and I feel he's right.
        // I'm taking them out for the time being.

        //uint_fast8_t toggle_reading();
        uint_fast8_t start_reading();
        uint_fast8_t stop_reading();

        uint_fast8_t set_enter( bool on );
        uint_fast8_t set_exit( bool on );
        uint_fast8_t set_self_filter( bool on=true );

        uint_fast8_t reading_status();

        bool is_reading();

        uint_fast8_t configure( uint_fast8_t flags );

        void update_filter();

        // I need this to be a pointer to check for nullity.

        Sensor_Data * read_syscall();

    protected:

        // Inherited from syscall_reader.h

        bool tracing_event( std::string event_path );
        bool filtering_self( std::string event_path );

        static Linux_Syscall_Reader * lsr_instance;

        Linux_Syscall_Reader( uint_fast8_t flags=READER_DEFAULT );

        bool write_to_file( std::string filename, std::string output );
        bool append_to_file( std::string filename, std::string output );
        bool file_write( std::string filename, std::string output, std::ios_base::openmode mode=std::ofstream::out );
        
        std::string file_readline( std::string filename );

        void clear_file( std::string file_name );

        std::string build_filter();

        std::vector<std::string> filter_files = { FILTER_SYS_ENTER, FILTER_SYS_EXIT };

        std::string os = "linux";
};
