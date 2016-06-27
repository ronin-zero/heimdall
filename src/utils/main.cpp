/*
 *  File Name : main.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Mon 27 Jun 2016 04:54:54 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "command_line_parser.h"

int main( int argc, char** argv ){

    Command_Line_Parser parser;

    parser.parse_args( argc, argv );

    parser.print_help();

    return 0;

}
