/*
 *  File Name : main.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Thu 30 Jun 2016 11:22:07 PM PDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "command_line_parser.h"

std::vector<string> opt_flags = { "-o", "-n", "-p", "-c", "-f", "-t", "-s", "-a", "-o", "--flags=", "--separator=" };
std::vector<string>

int main( int argc, char** argv ){

    Command_Line_Parser parser( argc, argv, opt_flags );

//    parser.parse_args( argc, argv );

    parser.print_help();

    return 0;
}
