/*
 *  File Name : trace_reader.h
 *  
 *  Creation Date : 03-01-2017
 *
 *  Last Modified : Mon 06 Mar 2017 02:18:55 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <regex>
#include "utils/ascii_operations.h"

static const std::string PROCESS_NAME_REG   =   "([[:graph:]]*)";
static const std::string PID_REG            =   "([[:digit:]]+)?";
static const std::string CPU_REG            =   "([[:digit:]]{3})?";
static const std::string TRACE_FLAGS_REG    =   "([.d][.Nnp][.Hhs][.[:digit:]])?";
static const std::string TIMESTAMP_REG      =   "([[:digit:]]+\\.[[:digit:]]+)?";
static const std::string SYSCALL_REG        =   "([[:digit:]]+)?";
static const std::string SYSCALL_ARG_REG    =   "(?:[[:xdigit:]]{1,8}(?:, )?){6}";
//static const std::string SYSCALL_ARGS   =   "(\\(" + SYSCALL_ARG + "\\))";
static const std::string SYSCALL_ARGS_REG   =   "(\\([, [:xdigit:]]*\\))?";

class Trace_Reader{

    public:

        Trace_Reader( std::string file_name, uint_fast8_t separator = ',' );
        ~Trace_Reader();

        bool has_next();
        int_fast32_t next_syscall();

        void dump_file( std::string out_file );

    protected:

        std::string _file_name;

        std::string regex_string;

        std::ifstream trace_file;

        std::regex trace_regex;

        uint_fast32_t line_count;

        size_t _syscall_position;
        uint_fast8_t _separator;

        std::string generate_regex_string( uint_fast8_t separator );

        //bool set_parameters();

        std::string get_syscall_field( std::string line );
};
