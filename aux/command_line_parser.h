/*
 *  File Name : command_line_arg_parser.h
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Wed 29 Jun 2016 10:24:04 PM PDT
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

        Command_Line_Parser( int argc, char** argv);
        ~Command_Line_Parser() {}

        bool contains_arg( std::string arg );
        bool contains_option( std::string opt );
        bool contains_any( std::vector<std::string> args );

        int_fast64_t arg_index( std::string arg, uint_fast32_t pos=0 );
        int_fast64_t option_index( std::string opt, uint_fast32_t pos=0 );

        int_fast32_t get_option_value( std::string arg );
        std::string get_option_string ( std::string arg );

        void print_help();
        void print_usage();

        //void parse_args( uint_fast32_t argc, char** argv );

    private:

        std::vector<std::string> arguments;

};
