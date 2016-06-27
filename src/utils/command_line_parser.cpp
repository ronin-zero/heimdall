/*
 *  File Name : command_line_parser.cpp
 *  
 *  Creation Date : 27-06-2016
 *
 *  Last Modified : Mon 27 Jun 2016 05:01:10 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "command_line_parser.h"

using namespace std;

void Command_Line_Parser::parse_args( uint_fast32_t argc, char** argv ){

    cout << "Number of arguments: " << argc << endl;

    cout << "They are as follows: " << endl;

    for ( uint_fast32_t i = 0; i < argc; i++ )
    {
        cout << "arg" << i << ": " << argv[i] << "  ";
    }

    cout << endl;
}

void Command_Line_Parser::print_help(){

    cout << "HEIMDALL syscall-sensor" << endl << endl;

    cout << "v 1.0.0" << endl << endl;

    cout << "DESCRIPTION" << endl;

    cout << string( 8, ' ' ) << "This program traces raw system calls and logs them to a specified output file." << endl;
    cout << string( 8, ' ' ) << "See OPTIONS for what data can be traced and logged." << endl;
    cout << string( 8, ' ' ) << "See COMMANDS for what actions can be taken." << endl;
    cout << string( 8, ' ' ) << "See DEFAULTS for the behavior when no options are set." << endl << endl;

    cout << "USAGE" << endl;

    cout << string( 8, ' ' ) << "syscall-sensor [COMMAND] [OPTION]" << endl << endl;

    cout << "COMMANDS" << endl;

    cout << string( 8, ' ' ) << setfill('.') << setw(20) << left << "start" ;
    cout << "Starts tracing accorinding to the given OPTIONS." << endl;
    cout << string( 28, ' ' ) << "Begins reading system call data from the trace pipe and logging it to the specified output file." << endl;


}
