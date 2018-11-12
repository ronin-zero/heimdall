/*
 *  File Name : linux_syscall_constants.h
 *  
 *  Creation Date : 11-12-2018
 *
 *  Last Modified : Mon 12 Nov 2018 03:53:26 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <string>

namespace Linux_Syscall_Constants{

    static const std::string TASK_REG           =   "([[:graph:][:space:]]*)";
    static const std::string PID_REG            =   "([[:digit:]]+)";
    static const std::string CPU_REG            =   "(\\[[[:digit:]]{3}\\])";
    static const std::string TRACE_FLAGS_REG    =   "([dX.][Nnp.][ZzHhs.][.[:digit:]])";
    static const std::string TIMESTAMP_REG      =   "([[:digit:]]+\\.[[:digit:]]+)";
    static const std::string SYSCALL_REG        =   "([[:digit:]]+)";
    static const std::string SYSCALL_ARGS_REG   =   "(\\([[:space:]]*[[:xdigit:]]+(?:,[[:space:]]*[[:xdigit:]]+)*[[:space:]]*\\))";

    static const std::string SYS_ENTER_REG      =   "(sys_enter:[[:space:]]+NR)";

    static const std::string FTRACE_REG         =   "^[[:space:]]*" + TASK_REG + "-" + PID_REG + 
        "[[:space:]]*" + CPU_REG + "[[:space:]]*" + TRACE_FLAGS_REG +
        "[[:space:]]*" + TIMESTAMP_REG + ":[[:space:]]*" + SYS_ENTER_REG +
        "[[:space:]]*" + SYSCALL_REG + "[[:space:]]*" + SYSCALL_ARGS_REG +
        "[[:space:]]*$";

    static const size_t FULL_INDEX          =   0;
    static const size_t TASK_INDEX          =   1;
    static const size_t PID_INDEX           =   2;
    static const size_t CPU_INDEX           =   3;
    static const size_t TRACE_FLAGS_INDEX   =   4;
    static const size_t TIMESTAMP_INDEX     =   5;
    static const size_t FUNCTION_INDEX      =   6;
    static const size_t SYSCALL_INDEX       =   7;
    static const size_t ARGS_INDEX          =   8;

    static const size_t MATCH_SIZE          =   9;
}
