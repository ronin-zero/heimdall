#pragma once

#include <vector>
#include <sys/types.h>  // Needed for get_pid and pid_t
#include <unistd.h>     // Needed for get_pid and pid_t

#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"

static const string FTRACE_DIR =        "/sys/kernel/debug/tracing/";
static const string TRACE =             "trace";
static const string TRACING_ON =        "tracing_on";
static const string RAW_SYSCALLS_DIR =  "raw_syscalls/";
static const string SYS_ENTER_DIR =     "sys_enter/";
static const string SYS_EXIT_DIR =      "sys_exit/";
static const string EVENTS_DIR =        "events/";
static const string TRACE_PIPE =        "trace_pipe";
static const string CURRENT_TRACER =    "current_tracer";
static const string ENABLE =            "enable";
static const string NO_TRACE =          "set_ftrace_notrace";
static const string FILTER  =           "filter";
static const string FILTER_SYS_ENTER =  FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_ENTER_DIR + FILTER;
static const string FILTER_SYS_EXIT =   FTRACE_DIR + EVENTS_DIR + RAW_SYSCALLS_DIR + SYS_EXIT_DIR + FILTER;

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

        // I need this to be a pointer to check for nullity.

        Sensor_Data * read_syscall();

        uint_fast8_t configure( uint_fast8_t flags );

    protected:

        static Linux_Syscall_Reader * lsr_instance;

        Linux_Syscall_Reader( uint_fast8_t flags=READER_DEFAULT );

        bool write_to_file( string filename, string output );
        bool append_to_file( string filename, string output );
        bool file_write( string filename, string output, std::ios_base::openmode mode=std::ofstream::out );

        void clear_file( string file_name );

        std::vector<string> filter_files = { FILTER_SYS_ENTER, FILTER_SYS_EXIT };

        string os = "linux";
};
