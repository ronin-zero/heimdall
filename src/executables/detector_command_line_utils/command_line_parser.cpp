/*
 *  File Name : command_line_parser.cpp
 *  
 *  Creation Date : 06-27-2016
 *
 *  Last Modified : Wed 08 Mar 2017 12:13:31 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "command_line_parser.h"

Command_Line_Parser::Command_Line_Parser( int argc, char** argv ) : arg_flags( { "-d", "-l", "-m", "-n", "-t", "-s" } ),
                                                                    //opt_flags( { "--model=", "--trace=", "--daemon=" } ),
                                                                    opt_flags(),
                                                                    commands( { "start", "stop", "status" } ){

    if ( argc <= 0 )
    {
        std::cerr << "ERROR: There are no arguments.  Please check your main program." << std::endl;
    }
    else
    {
        program_name = parse_program_name( argv[0] );

        for ( int i = 1; i < argc; i++ ){

            // Convert each char* in argv into a std::string...
            std::string tmp( argv[i] );

            // Store the std::string in a vector.
            arguments.push_back( tmp );
        }
    }
}

bool Command_Line_Parser::contains_arg( std::string arg ){

    return ( arg_index( arg ) >= 0 );
}

bool Command_Line_Parser::contains_option( std::string opt ){

    return ( option_index( opt ) >= 0  );
}

bool Command_Line_Parser::contains_any( std::vector<std::string> args ){

    for ( uint_fast32_t i = 0; i < args.size(); i++ )
    {
        if ( contains_arg( args[i] ) )
        {
            return true;
        }
    }

    return false;
}

std::string Command_Line_Parser::get_program_name(){

    return program_name;
}

int_fast64_t Command_Line_Parser::arg_index( std::string arg, uint_fast32_t pos ){

    int_fast64_t index = -1;

    for ( uint_fast32_t i = pos; i < arguments.size(); i++ )
    {
        if ( arguments[i] == arg )
        {
            return i;
        }
    }

    return index;
}

int_fast64_t Command_Line_Parser::option_index( std::string opt, uint_fast32_t pos ){

    int_fast64_t index = -1;

    for ( uint_fast32_t i = pos; i < arguments.size(); i++ )
    {
        if ( arguments[i].find( opt ) != std::string::npos )
        {
            return i;
        }
    }

    return index;
}

int_fast32_t Command_Line_Parser::get_option_value( std::string arg ){

    // TODO: convert the value passed to this option to an unsigned int.
    // Accept both hex and base 10 values.

    int_fast32_t opt_val = -1;

    std::string opt_arg = get_option_string( arg );

    if ( ASCII_Operations::is_hex_byte( opt_arg ) )
    {
        opt_val = ASCII_Operations::hex_byte_val( opt_arg );
    }
    else if ( ASCII_Operations::is_number( opt_arg ) )
    {
        opt_val = ASCII_Operations::to_int( opt_arg );
    }

    return opt_val;
}

std::string Command_Line_Parser::get_option_string( std::string arg ){

    // TODO: Parse the argument to get the value of the option after
    // the '=' character.

    std::size_t opt_start = arg.find("=") + 1;

    std::string raw_opt_string = arg.substr( opt_start );

    std::string opt_string = replace_tab_char( raw_opt_string ); 

    if ( opt_string.length() < 1 )
    {
        std::cerr << "FAILURE - Invalid argument to option: " << arg.substr( 0, opt_start - 1 ) << std::endl;
        std::cerr << "Full argument was: " << arg << std::endl;
        std::cerr << "opt_string was: " << opt_string << std::endl;
    }

    return opt_string;
}

/*
   void Command_Line_Parser::parse_args( uint_fast32_t argc, char** argv ){

   std::cout << "Number of arguments: " << argc << std::endl;

   std::cout << "They are as follows: " << std::endl;

   for ( uint_fast32_t i = 0; i < argc; i++ )
   {
   std::cout << "arg" << i << ": " << argv[i] << "  ";
   }

   std::cout << std::endl;
   }*/

void Command_Line_Parser::print_help(){

    std::cout << "HEIMDALL syscall-detector" << std::endl << std::endl;

    std::cout << "v 1.0.0" << std::endl << std::endl;

    std::cout << "DESCRIPTION" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "This program monitors traces of n-grams of syscalls and logs them to a file." << std::endl;
    std::cout << std::string( 8, ' ' ) << "See OPTIONS for what configuration options are available for the detector." << std::endl;
    std::cout << std::string( 8, ' ' ) << "See COMMANDS for what actions can be taken." << std::endl;
    std::cout << std::string( 8, ' ' ) << "See DEFAULTS for the behavior when no options are set." << std::endl << std::endl;

    std::cout << "USAGE" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "syscall-detector [COMMAND] [OPTION]" << std::endl << std::endl;

    std::cout << "COMMANDS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setfill('.') << std::setw(20) << std::left << "start" ;
    std::cout << "Starts detecting for anomalies accorinding to the given OPTIONS." << std::endl;
    std::cout << std::string( 28, ' ' ) << "If a support vector machine model is trained, it begins monitoring system calls." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setfill('.') << std::setw(20) << std::left << "stop" ;
    std::cout << "Stops the detector (if it is running)." << std::endl;
    std::cout << std::string( 28, ' ' ) << "Cleanly closes the log file, saves the model (optional), and cleans up unfreed memory (if needed)." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << std::setfill('.') << std::setw(20) << std::left << "status";
    std::cout << "Shows the status of the detector." << std::endl;
    std::cout << std::string( 28, ' ' ) << "Status information includes whether the detector is running, ngram length, trace window length, etc." << std::endl << std::endl;


    std::cout << "OPTIONS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "This portion of the manual is broken into five sections: " << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "1. DETECTION LOG - How to specify the file to which output should be logged." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "2. TRACE WINDOW - An explanation of what the trace window is and what options it offers." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "3. NGRAMS - An explanation of what an ngram is and how to specify its length." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "4. TRACE INPUT FILE - Explains the input trace file and its options." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "5. TRACE OUTPUT FILE - Explains where the current syscall trace is recorded." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "6. MODEL FILE - Explains the input model file and its options." << std::endl << std::endl;
    
    std::cout << std::string( 8, ' ' ) << "7. DAEMONIZE - Explains how to run as a daemon." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "8. DEFAULT CONFIGURATION - The configuration of the detector if it is run with no specified" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "1. DETECTION LOG" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-d <detection_log_file>" << std::endl;

    std::cout << std::string( 11, ' ' ) << "Writes the detector's findings to the file specified by \'detection_log_file\'.  The path is relative from wherever" << std::endl;
    std::cout << std::string( 11, ' ' ) << "the command to run the detector was called.  The filename cannot contain white space.  Everything logged to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "this file will have a timestamp." << std::endl;
    std::cout << std::string( 11, ' ' ) << "WARNING: This program should be considered a rough beta.  It strongly recommended that you avoid using" << std::endl;
    std::cout << std::string( 11, ' ' ) << "meta characters or characters with special significance to the terminal.  The input is not guaranteed to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "be sanitized and any such behavior is undefined AT BEST.  The user assumes all risk for using this program." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "2. TRACE WINDOW" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-l <length>" << std::endl;

    std::cout << std::string( 11, ' ' ) << "The set of contiguous syscalls that are broken into ngrams.  Consider the following example trace window:" << std::endl << std::endl;

    std::cout << std::string( 15, ' ' ) << "( 32, 351, 201, 128, 16, 5 )" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "This would be a trace window of length 6 (-l 6) as it contains 6 system calls." << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "These are then broken into ngrams and the frequency of each is put into a support vector." << std::endl << std::endl;
    
    std::cout << std::string( 8, ' ' ) << "3. NGRAMS" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-n <n>" << std::endl << std::endl;

    std::cout << std::string( 11, ' ') << "This is the length of the ngram to be used.  With the previous trace window example of:" << std::endl << std::endl;

    std::cout << std::string( 15, ' ') << "( 32, 351, 201, 128, 16, 5 )" << std::endl << std::endl;

    std::cout << std::string( 11, ' ') << "An ngram length of 3 (-n 3) would give the following ngrams:" << std::endl << std::endl;

    std::cout << std::string( 15, ' ') << "( [ 32, 351, 201 ], [ 351, 201, 128 ], [ 201, 128, 16 ], [ 128, 16, 5 ] ) " << std::endl << std::endl;

    std::cout << std::string( 11, ' ') << "Ngrams are effectively sliding windows over the trace window." << std::endl << std::endl;
    
    std::cout << std::string( 8, ' ' ) << "4. TRACE INPUT FILE" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-t <trace_file>" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "This is where one specifies the file containing the trace data from which the detector should train and build" << std::endl;
    std::cout << std::string( 11, ' ' ) << "its svm model.  Currently, only the default format produced by the syscall-sensor program with default settings" << std::endl;
    std::cout << std::string( 11, ' ' ) << "is supported.  That format is as follows:" << std::endl << std::endl;

    std::cout << std::string( 15, ' ' ) << "TASK,PID,TIMESTAMP,SYSCALL" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "5. TRACE INPUT FILE" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-o <trace_log>" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "This is where the syscalls traced during this session will be logged.  For now, they will be logged in the default format" << std::endl;
    std::cout << std::string( 11, ' ' ) << "used by the syscall-sensor program ( TASK,PID,TIMESTAMP,SYSCALL).  If no argument is supplied, they will be logged to" << std::endl;
    std::cout << std::string( 11, ' ' ) << "trace.log" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "6. MODEL FILE" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "-m <model_file>" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "<model_file> specifies the file containing the svm model to use rather than training from a trace." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "7. DAEMON" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "--daemon=[ON|OFF|0|1]" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "By default, this program runs as a daemon-like background process.  Advanced users may wish to disable that" << std::endl;
    std::cout << std::string( 11, ' ' ) << "functionality for debugging or other reasons by passing option \"OFF\" (do not do this unless" << std::endl;
    std::cout << std::string( 11, ' ' ) << "you know what you are doing)." << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "8. DEFAULT CONFIGURATION" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "When run with no arguments, it is equivalent to entering with arguments: " << std::endl << std::endl;

    std::cout << std::string( 15, ' ' ) << "syscall-detector start -n 2 -l -d syscall_detector.log" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "Note that this will cause the detector to train in real-time and is currently not supported." << std::endl << std::endl;
    
    std::cout << std::string( 8, ' ' ) << "Please report bugs to:" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << "ronin-zero@github.com" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "The project's main repository can be found at:" << std::endl << std::endl;
    std::cout << std::string( 11, ' ' ) << "https://github.com/ronin-zero/heimdall" << std::endl << std::endl;

    std::cout << "Written by: 浪人ー無, 2016" << std::endl << std::endl;
}

void Command_Line_Parser::print_usage(){

    std::cout << std::endl << "USAGE:" << std::endl;
    std::cout << std::string( 8, ' ' ) << "syscall-detector COMMAND [OPTIONS]" << std::endl << std::endl;

    std::cout << std::string( 8, ' ' ) << "COMMANDs" << std::endl << std::endl;
    std::cout << std::string( 11, ' ' ) << std::setw(40) << std::setfill(' ') << std::left << "start" << "start the detector with [OPTIONS]" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(40) << std::setfill(' ') << std::left << "stop" << "stop the detector" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(40) << std::setfill(' ') << std::left << "status" << "check detector status (NOT IMPLEMENTED)" << std::endl;
    std::cout << std::endl;

    std::cout << std::string( 8, ' ' ) << "OPTIONS" << std::endl << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-d <detection_log_file>";
    std::cout << "Specify the name of the file to which the detector logs its findings (default filename: syscall_detector.log)" << std::endl;
    
    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-l <length>";
    std::cout << "Specify value of \'l\' (trace window length, default length: 100)" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-m <model_file>";
    std::cout << "Specify the name of the file containing the saved svm model" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-n <n>";
    std::cout << "Specify value of \'n\' (size of the ngram, default n value is 2)" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-t <trace_file>";
    std::cout << "Specify the name of the file contraining the trace to use for training" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "-o <trace_log>";
    std::cout << "Specify the name of the file to which syscalls will be logged (default filename: trace.log)" << std::endl;

    std::cout << std::string( 11, ' ' ) << std::setw(20) << std::setfill(' ') << std::left << "--daemon=[ON|OFF]";
    std::cout << "run the detector as a daemon-like background process (ON; the default) or as a regular application (OFF). (ADVANCED)" << std::endl << std::endl;
    
    std::cout << std::endl;

    std::cout << "Run with flag -h to view the help documentation (it is recommended that you pipe it to a pager" << std::endl;
    std::cout << "such as \'less\' for ease of reading.  Example: \'syscall-sensor -h | less\')." << std::endl;
}

// This function is to make sure that pairs of quotes are balanced.
// The user can specify any char at runtime, but the intention is
// to check for quotes.
bool Command_Line_Parser::check_balance( std::string input, int_fast8_t q ){

    int_fast32_t input_length = input.length();

    // If it's only one character long, it can't be a quotation mark.
    // If it's 0 characters long, it's not worth checking the rest.

    if ( input_length > 2 )
    {

        int_fast32_t first_index = input.find( q );
        int_fast32_t second_index = input.rfind( q );

        return  ( first_index == 0 && second_index == input_length - 1 );
    }

    return false;
}

std::string Command_Line_Parser::strip_endpoints( std::string input ){

    return input.substr( 1, input.length() - 2 );
}

std::string Command_Line_Parser::sanitize_input( std::string input ){

    std::string sanitized = "";

    if ( check_balance( input, '"' ) )
    {
        sanitized = strip_endpoints( input );
    }
/*    else if ( check_balance( input, '\'' ) )
    {
        sanitized = strip_endpoints( input );
        
        if ( sanitized.length() > 2 )
        {
            sanitized = "";
        }
        
    }*/
    else
    {
        sanitized = input;
    }
    std::cout << "Sanitized string is " << sanitized << std::endl;
    return sanitized;
}

std::string Command_Line_Parser::replace_tab_char( std::string input ){

    std::string output = "";

    std::string tab_char = "\\t";

    std::size_t tab_index = input.find( "\\t" );

    if ( tab_index == std::string::npos )
    {
        output = input;
    }
    else
    {
        output += input.substr( 0, tab_index );
        output += '\t';
        output += replace_tab_char( input.substr( tab_index + 2 ) );
    }

    return output;
}

std::string Command_Line_Parser::parse_program_name( char* arg ){

    std::string name( arg );

    std::string dot_slash = "./";

    std::size_t slash_pos = name.find( dot_slash );

    if ( slash_pos == std::string::npos )
    {
        return name;
    }

    else
    {
        return name.substr( slash_pos + dot_slash.length() );
    }
}

void Command_Line_Parser::print_args(){

    for ( uint_fast32_t i = 0; i < arguments.size(); i++ )
    {
        std::cout << "Arg #" << i << ": " << arguments[i] << std::endl;
    }
}

bool Command_Line_Parser::check_args(){

    uint_fast32_t commands = 0;
    uint_fast32_t errors = 0;

    bool valid_args = true;

    std::vector<uint_fast32_t> bad_args;

    for ( uint_fast32_t i = 0; i < arguments.size(); i++ )
    {
        if ( valid_command( arguments[i] ) )
        {
            commands++;
        }
        else if ( !( valid_option( arguments[i] ) || valid_arg( arguments[i] ) ) )
        {
            if ( i > 0 && arguments[ i -1 ] != "-o" )
            {
                errors++;
                bad_args.push_back(i);
            }
        }
    }

    if ( commands != 1 )
    {
        errors++;
    }

    if ( errors > 0 )
    {
        valid_args = false;

        std::cerr << "ERROR: " << errors << " errors in arguments." << std::endl;
        
        print_malformed_args( bad_args, commands );
    }

    return valid_args;
}

bool Command_Line_Parser::valid_command( std::string input ){

    for ( uint_fast32_t i = 0; i < commands.size(); i++ )
    {
        if ( input == commands[i] )
        {
            return true;
        }
    }

    return false;
}

bool Command_Line_Parser::valid_option( std::string input ){

    for ( uint_fast32_t i = 0; i < opt_flags.size(); i++ )
    {
        if ( input.find( opt_flags[i] ) == 0 && opt_flags[i].length() < input.length() )
        {
            return true;
        }
    }

    return false;
}

bool Command_Line_Parser::valid_arg( std::string input ) {

    for ( uint_fast32_t i = 0; i < arg_flags.size(); i++  )
    {
        if ( input == arg_flags[i] )
        {
            return true;
        }
    }

    return false;
}

void Command_Line_Parser::print_malformed_args( std::vector<uint_fast32_t> malformed_args, uint_fast32_t num_cmds ){

    if ( num_cmds == 0 )
    {
        std::cerr << "ERROR: No commands (start/status/stop) given." << std::endl;
    }
    
    if ( num_cmds > 1 )
    {
        std::cerr << "ERROR: Too many commands. Only one start/status/stop command may be given." << std::endl;
    }

    for ( uint_fast32_t i = 0; i < malformed_args.size(); i++ )
    {
        std::string tmp_arg = arguments[ malformed_args[i] ];

        std::cerr << "ERROR - Bad argument #" << i << " as argument " << malformed_args[i] << std::endl;

        if ( tmp_arg.length() <= 2 )
        {
            std::cerr << "Unrecognized argument \"" << tmp_arg << "\"" << std::endl;
        }
        else if ( tmp_arg.find( "--separator=" ) != std::string::npos )
        {
            std::string option = tmp_arg.substr( tmp_arg.find('=') + 1 );

            if ( option.length() < 1 )
            {
                std::cerr << "--separator requires an argument." << std::endl;
            }
            else
            {
                std::cerr << option << " is not a valid argument for option --separator" << std::endl;
            }
        }
        else if ( tmp_arg.find( "--flags=" ) != std::string::npos )
        {
            std::string option = tmp_arg.substr( tmp_arg.find('=') + 1 );

            if ( option.length() < 1 || ( !ASCII_Operations::is_hex_byte( option ) && !ASCII_Operations::is_number( option ) ) )
            {
                std::cerr << "--flags requires an argument of either a decimal or hexidecimal number between 0 and 255 (inclusive)" << std::endl;
            }
            else
            {
                std::cerr << "Argument \"" << option << "\" to option --flags is malformed." << std::endl;
            }
        }
        else
        {
            std::cerr << "Unspecified problem with argument " << tmp_arg << std::endl;
        }
    }

    print_usage();
}
