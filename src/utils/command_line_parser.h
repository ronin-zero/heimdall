/*
 *  File Name : command_line_arg_parser.h
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Wed 29 Jun 2016 10:11:41 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>
#include <iomanip>
#include <vector>

class Command_Line_Parser{

    public:

        Command_Line_Parser( int argc, char** argv) {} // TODO: FIXME. 
        ~Command_Line_Parser() {}

        bool contains_arg( std::string arg );
        bool contains_option( std::string option );
        bool contains_any( std::vector<std::string> args );

        int_fast32_t arg_index( std::string arg );
        int_fast32_t opt_index( std::string opt );

        // Review this one, you wrote it on your way out the door:

        bool verify_arguments(); // You may want more than a true/false check here.

        int_fast32_t get_option_value( std::string arg );
        std::string get_option_stringvalue( std::string arg );

        void print_help();

        void parse_args( uint_fast32_t argc, char** argv );

    private:

        std::vector<std::string> arguments;

};
