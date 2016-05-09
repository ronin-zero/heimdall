#pragma once

#include "syscall_reader.h"

class Linux_Syscall_Reader:public Syscall_Reader{
    
    const string FTRACE_DIR =       "/sys/kernel/debug/tracing/";
    const string TRACING_ON =       "tracing_on";
    const string RAW_SYSCALLS_DIR=  "raw_syscalls/";
    const string SYS_ENTER_DIR =   "sys_enter/";
    const string SYS_EXIT_DIR =     "sys_exit/";
    const string EVENTS_DIR =       "events/";
    const string TRACE_PIPE =       "trace_pipe";
    const string CURRENT_TRACER =   "current_tracer"
    const string ENABLE =           "enable"
    const string NOTRACE =          "set_ftrace_notrace"

    public:
        
    private:
        string os = "linux";
        string data_type = "syscalls"
}
