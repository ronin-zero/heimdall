/*
 *  File Name : command_line_arg_parser.h
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Fri 01 Jul 2016 09:12:16 PM EDT
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

#include "ascii_operations.h"

class Command_Line_Parser{

    public:

        Command_Line_Parser( int argc, char** argv );
        ~Command_Line_Parser() {}

        bool contains_arg( std::string arg );
        bool contains_option( std::string opt );
        bool contains_any( std::vector<std::string> args );

        uint_fast32_t num_args() { return arguments.size(); }

        std::string arg_at( uint_fast32_t index ) { return arguments.at( index ); }

        int_fast64_t arg_index( std::string arg, uint_fast32_t pos=0 );
        int_fast64_t option_index( std::string opt, uint_fast32_t pos=0 );

        int_fast32_t get_option_value( std::string arg );
        std::string get_option_string ( std::string arg );

        void print_help();
        void print_usage();

        //void parse_args( uint_fast32_t argc, char** argv );

    private:

        // This is to check that quotation marks are properly closed
        // if they are opened.  This could be used for any character
        // as the user may specify any character value for 'q' at
        // runtime, but the intention is that the user uses it to
        // check for balanced quotes.

        bool check_balance( std::string input, int_fast8_t q );

        std::string sanitize_input( std::string input );
        std::string strip_endpoints( std::string input );

        std::vector<std::string> arguments;
};
