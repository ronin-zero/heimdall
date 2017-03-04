/*
 *  File Name : trace_reader.h
 *  
 *  Creation Date : 03-01-2017
 *
 *  Last Modified : Sat 04 Mar 2017 12:40:54 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "utils/ascii_operations.h"

static const std::string PROCESS_NAME   =   "[[:graph:]]*";
static const std::string PID            =   "[[:digit:]]+";
static const std::string CPU            =   "[[:digit:]]{3}";
static const std::string TRACE_FLAGS    =   "[[.d][.Nnp][.Hhs][.[:digit:]]";
static const std::string TIMESTAMP      =   "[[:digit:]]+\\.[[:digit:]]+";
static const std::string SYSCALL        =   "[[:digit:]]+";
static const std::string SYSCALL_ARG    =   "[[:xdigit:]{1-8}[,?[:blank:]]?";
static const std::string SYSCALL_ARGS   =   "\\(" + SYSCALL_ARG + "{6}\\)";

class Trace_Reader{

    public:

        Trace_Reader( std::string file_name, uint_fast8_t separator = ',', size_t syscall_position = std::string::npos );
        ~Trace_Reader();

        bool has_next();
        int_fast32_t next_syscall();

    protected:

        std::string _file_name;

        std::ifstream trace_file;

        uint_fast32_t line_count;

        size_t _syscall_position;
        uint_fast8_t _separator;

        //bool set_parameters();

        std::string get_syscall_field( std::string line );
};
