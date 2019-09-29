/*
 *  File Name : command_line_parser.h
 *  
 *  Creation Date : 09-29-2019
 *
 *  Last Modified : Sun 29 Sep 2019 04:05:22 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <iostream>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "utils/ascii_operations.h"

class Command_Line_Parser{

    public:

        Command_Line_Parser( int32_t argc, char** argv );
        ~Command_Line_Parser() {};

        bool contains_arg( std::string arg );
        bool contains_option( std::string opt );
        bool contains_any( std::vector<std::string> args );

        uint_fast32_t num_args() { return arguments.size(); }

        std::string arg_at( uint_fast32_t index ) { return arguments.at( index ); }
        std::string get_program_name() { return program_name; }

        int_fast64_t arg_index( std::string arg, uint_fast32_t pos );


};
