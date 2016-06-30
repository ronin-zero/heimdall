/*
 *  File Name : main.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Wed 29 Jun 2016 10:11:18 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "command_line_parser.h"

int main( int argc, char** argv ){

    Command_Line_Parser parser( argc, argv );

//    parser.parse_args( argc, argv );

    parser.print_help();

    return 0;

}
