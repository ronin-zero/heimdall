#pragma once

#include "sensors/syscall_sensors/syscall_readers/syscall_reader.h"

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
/*static const string FTRACE_DIR = "/sys/kernel/debug/tracing/";
  static const string TRACING_ON;
  static const string RAW_SYSCALLS_DIR;
  static const string SYS_ENTER_DIR;
  static const string SYS_EXIT_DIR;
  static const string EVENTS_DIR;
  static const string TRACE_PIPE;
  static const string CURRENT_TRACER;
  static const string ENABLE;
  static const string NO_TRACE;*/

class Linux_Syscall_Reader:public Syscall_Reader{

    public:
        static Linux_Syscall_Reader * get_instance( uint_fast8_t flags=READER_DEFAULT );
        ~Linux_Syscall_Reader();

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
         */

        uint_fast8_t set_reading( bool on );
        uint_fast8_t toggle_reading();
        uint_fast8_t start_reading();
        uint_fast8_t stop_reading();

        uint_fast8_t set_enter( bool on );
        uint_fast8_t set_exit( bool on );
        uint_fast8_t set_self_filter( bool on=true );

        uint_fast8_t reading_status();

        bool is_reading();

        // I need this to be a pointer to check for nullity.

        Sensor_Data * read_syscall();

        void read();

    protected:

        static Linux_Syscall_Reader * lsr_instance;

        Linux_Syscall_Reader( uint_fast8_t flags=READER_DEFAULT );

        bool write_to_file( string filename, string output );
        bool append_to_file( string filename, string output );
        bool file_write( string filename, string output, std::ios_base::openmode mode=std::ofstream::out );

        string os = "linux";
};
