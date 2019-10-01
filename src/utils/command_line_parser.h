/*
 *  File Name : command_line_parser.h
 *  
 *  Creation Date : 09-29-2019
 *
 *  Last Modified : Tue 01 Oct 2019 02:20:32 AM EDT
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

        virtual Command_Line_Parser(int32_t argc, char** argv) = 0;
        ~Command_Line_Parser() {};

        bool contains_arg(std::string arg);
        bool contains_option(std::string opt);
        bool contains_any(std::vector<std::string> args);

        size_t num_args() { return arguments.size(); }

        std::string arg_at(size_t index) { return arguments.at(index); }
        std::string get_program_name() { return program_name; }

        size_t arg_index(std::string arg, size_t pos=0);
        size_t option_index(std::string opt, size_t pos=0);

        int_fast32_t get_option_value(std::string opt);
        std::string get_option_string(std::string opt);

        virtual void print_usage() = 0;
        virtual bool check_args() = 0;

    protected:

        std::vector<std::string> arguments; // This is just each token in argv as a string in a vector
        std::string program_name; // This will be the name of the program.  Set as argv[0] in the constructor.

        std::vector<std::string> short_opts;
        std::vector<std::string> long_opts;
        std::vector<std::string> short_opt_args;
        std::vector<std::string> long_opt_args;
        std::vector<std::string> commands;


        

};
