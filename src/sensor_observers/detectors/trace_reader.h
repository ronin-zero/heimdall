/*
 *  File Name : trace_reader.h
 *  
 *  Creation Date : 03-01-2017
 *
 *  Last Modified : Thu 02 Mar 2017 02:35:22 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "utils/ascii_operations.h"

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
