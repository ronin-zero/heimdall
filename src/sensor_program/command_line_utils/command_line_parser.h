/*
 *  File Name : command_line_arg_parser.h
 *  
 *  Creation Date : 06-27-2016
 *
 *  Last Modified : Tue 31 Jan 2017 03:45:25 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>
#include <iomanip>
#include <vector>

#include "utils/ascii_operations.h"

class Command_Line_Parser{

    public:

        Command_Line_Parser( int argc, char** argv );
        ~Command_Line_Parser() {};

        bool contains_arg( std::string arg );
        bool contains_option( std::string opt );
        bool contains_any( std::vector<std::string> args );

        uint_fast32_t num_args() { return arguments.size(); }

        std::string arg_at( uint_fast32_t index ) { return arguments.at( index ); }
        std::string get_program_name();

        int_fast64_t arg_index( std::string arg, uint_fast32_t pos=0 );
        int_fast64_t option_index( std::string opt, uint_fast32_t pos=0 );

        int_fast32_t get_option_value( std::string arg );
        std::string get_option_string ( std::string arg );

        void print_help();
        void print_usage();

        void print_args();

        bool check_args();
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
        std::string replace_tab_char( std::string input );
        std::string parse_program_name( char* arg );

        std::vector<std::string> arguments;
        std::string program_name;

        std::vector<std::string> arg_flags = { "-o", "-n", "-p", "-c", "-f", "-t", "-s", "-a", "-h" };
        std::vector<std::string> opt_flags = { "--separator=", "--flags=", "--daemon=" };
        std::vector<std::string> commands = { "start", "stop", "status" };

        bool valid_command ( std::string input );
        bool valid_option ( std::string input );
        bool valid_arg ( std::string input );

        void print_malformed_args( std::vector<uint_fast32_t> malformed_args, uint_fast32_t num_cmds );
};
