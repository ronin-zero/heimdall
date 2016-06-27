/*
 *  File Name : command_line_arg_parser.h
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Mon 27 Jun 2016 04:57:35 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <iomanip>

class Command_Line_Parser{

    public:

        Command_Line_Parser() {}
        ~Command_Line_Parser() {}

        void print_help();

        void parse_args( uint_fast32_t argc, char** argv );

    private:


};
