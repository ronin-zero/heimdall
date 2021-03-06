/*
 *  File Name : trace_reader.cpp
 *  
 *  Creation Date : 03-01-2017
 *
 *  Last Modified : Thu 29 Jun 2017 11:28:35 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "trace_reader.h"

//syscall_position should be the 0-indexed position of the field containing the syscall.

Trace_Reader::Trace_Reader( std::string file_name, uint_fast8_t separator  ) : trace_file( file_name ) {

    _separator = separator;

    _file_name = file_name;

    regex_string = generate_regex_string( separator );

    // TODO: This should probably be in its own method, so, put it in its own method.

    try {
        trace_regex = std::regex( regex_string );
    } catch( std::regex_error &e ){

        std::cerr << "REGEX ERROR: ";

        switch( e.code() )
        {
            case std::regex_constants::error_collate:
                std::cerr << "error_collate";
                break;
            case std::regex_constants::error_ctype:
                std::cerr << "error_ctype";
                break;
            case std::regex_constants::error_escape:
                std::cerr << "error_escape";
                break;
            case std::regex_constants::error_backref:
                std::cerr << "error_backref";
                break;
            case std::regex_constants::error_brack:
                std::cerr << "error_brack";
                break;
            case std::regex_constants::error_paren:
                std::cerr << "error_paren";
                break;
            case std::regex_constants::error_brace:
                std::cerr << "error_brace";
                break;
            case std::regex_constants::error_badbrace:
                std::cerr << "error_badbrace";
                break;
            case std::regex_constants::error_range:
                std::cerr << "error_range";
                break;
            case std::regex_constants::error_space:
                std::cerr << "error_space";
                break;
            case std::regex_constants::error_badrepeat:
                std::cerr << "error_badrepeat";
                break;
            case std::regex_constants::error_complexity:
                std::cerr << "error_complexity";
                break;
            case std::regex_constants::error_stack:
                std::cerr << "error_stack";
                break;
            default:
                std::cerr << "¯\\_(ツ)_/¯";
        }

        std::cerr << std::endl;
    }

    line_count = 0;
}

Trace_Reader::~Trace_Reader(){

    trace_file.close();
}

bool Trace_Reader::has_next(){

    return trace_file.good();
}

void Trace_Reader::dump_file( std::string out_file ){

    std::string line;
    std::string syscall_string;

    uint_fast32_t count = 0;

    std::ofstream my_out( out_file );

    std::cout << "Regex is: " << regex_string << std::endl << std::endl;

    while ( std::getline( trace_file, line ) )
    {
        my_out << count << ": " << line << " - ";

        if ( std::regex_match( line, trace_regex ) )
        {
            my_out << "MATCH";

            syscall_string = line.substr( line.rfind(',')+1 );

            my_out << " - Syscall: " << syscall_string;
        }
        else
        {
            my_out << "ERROR.";
        }

        my_out << std::endl;

        count++;
    }
}

// TODO: Make this regex more robust.  For now, it will only recognize the default pattern
// generated by the sensor.

std::string Trace_Reader::generate_regex_string( uint_fast8_t separator ){

    std::string sep_string = std::iscntrl( separator ) ? "\\" + separator : std::string(1, separator);

    std::string field_separator = sep_string + "(?!$)";

    std::string r_string = "^" + PROCESS_NAME_REG + field_separator 
        + PID_REG + field_separator
        //+ CPU_REG + field_separator
        //+ TRACE_FLAGS_REG + field_separator
        + TIMESTAMP_REG + field_separator
        + SYSCALL_REG + "$";// field_separator
    //+ SYSCALL_ARGS_REG + "$";

    //+ field_separator + PID_REG + field_separator + CPU_REG + field_separator + TRACE_FLAGS_REG + field_separator
    //   + TIMESTAMP_REG + field_separator + SYSCALL_REG + field_separator + SYSCALL_ARGS_REG;

    /*    std::string r_string = "^" + PROCESS_NAME_REG + field_separator + PID_REG + field_separator + CPU_REG + field_separator + TRACE_FLAGS_REG + field_separator
          + TIMESTAMP_REG + field_separator + SYSCALL_REG + field_separator + SYSCALL_ARGS_REG + "$";*/

    return r_string;
}


int_fast32_t Trace_Reader::next_syscall(){

    // TODO: make this return a meaningful value here.

    int_fast32_t syscall = -1;

    std::string line;
    std::string syscall_field;

    if ( !has_next() )
    {
        std::cerr << "ERROR: file " << _file_name << " has no more lines." << std::endl;
    } 

    else if ( getline( trace_file, line ) )
    {
        syscall_field = get_syscall_field( line );

        if ( ASCII_Operations::is_number( syscall_field ) )
        {
            syscall = ASCII_Operations::to_int( syscall_field );
        }
        else
        {
            std::cerr << "ERROR: the system call field is not a number!" << std::endl;
            std::cerr << '\t' << "File Name: " << _file_name << std::endl;
            std::cerr << '\t' << "System call field: " << syscall_field << std::endl;
            std::cerr << '\t' << "Line " << line_count << ": " << line << std::endl;
            //std::cerr << '\t' << "Syscall field position: " << _syscall_position << std::endl;
            std::cerr << '\t' << "Separator: " << _separator << std::endl;
        }

        line_count++;
    }
    else
    {
        std::cerr << "ERROR: Could not read line from file " << _file_name << "." << std::endl;
    }

    return syscall;
}

std::string Trace_Reader::get_syscall_field( std::string line ){

    size_t field_start = 0;

    std::string syscall_portion;


    field_start = line.rfind( _separator );

    if ( field_start == std::string::npos )
    {
        syscall_portion = line;
    }
    else
    {
        syscall_portion = line.substr( field_start + 1 );
    }

    return syscall_portion;
}
/*
   bool Trace_Reader::set_parameters(){

   trace_file.open( _file_name );

   if ( !trace_file.good() )
   {
   if ( trace_file.open() )
   {
   trace_file.close();
   }

   std::cerr << "ERROR: Problem with the file.  It wasn't good." << std::endl;

   return false;
   }

   std::string test_string;

   if ( getline( trace_file, test_string ) )
   {
// We don't need to read anything else from this file
// for the rest of this method.

trace_file.close();

if ( _syscall_position == std::string::npos )
{

// First check to see if the separator even appeas in the record string by
// searching from the end of the string (rfind).

size_t sep_index = test_string.rfind( _separator );

// If the separator isn't found, it's possible that the line only contains the syscall number
// so check to see if test_string is a number itself.  Regardless of whether it's a number,
// return the result of is_number on test_string.

if ( sep_index == std::string::npos )
{
if ( is_number( test_string ) )
{
return true;
}
else
{
std::cerr << "ERROR: The line provided is not a valid trace record." << std::endl;
std::cerr << "OFFENDING LINE: " << test_string << std::endl;

return false;
}
}

// Attempt to pull the last field as delimited by the _separator character

std::string syscall_portion = test_string.substr( sep_index + 1 );

// Check if the string in this field is a number as defined in ascii_operations.[cpp/h]
// Granted, this is not fool-proof; the PID will also be a number.  We will search from
// the end of the line using rfind.  As written, the only option that may appear after 
// the system call number is the arguments and those are disabled by default.
// If both the PID and the system call number are enabled when collecting the trace,
// the system call number will appear AFTER the PID on the line and will be found
// first when using rfind.  Theoretically, the only way this could go wrong is if one of
// the arguments to the system call is a number or if the option to record system
// call numbers was not enabled when collecting the trace.

if ( is_number( syscall_portion ) )
{
return true;
}

// If we've reached this point, we now need to iterate through the fields, starting from the right
// until we either find a field that looks like a syscall number or we run out of fields,
// returning true or returning false as appropriate.

size_t next_index = sep_index;

if ( sep_index > 0 )
{
    sep_index = test_string.rfind( _separator, sep_index - 1 );
}

while ( sep_index != std::string::npos && sep_index > 0 )
{
    sep_after_call++;

    syscall_portion = test_string.substr( sep_index + 1, next_index - ( sep_index + 1 ) );

    if ( is_number( syscall_portion ) )
    {
        // This means we've found it. 
        // We need to set its position by counting how many separators appear before the field.

        size_t tmp_syscall_position = 1;

        while ( sep_index > 0 && test_string.rfind( _separator, sep_index -1 ) != std::string::npos  )
        {
            tmp_syscall_position++;

            sep_index = test_string.rfind( _separator, sep_index - 1 ); 
        }

        _syscall_position = tmp_syscall_position;

        return true;
    }

    sep_index = test_string.rfind( _separator, sep_index - 1 );
}

// That's all she wrote, this isn't formatted right.

std::cerr << "ERROR: The line provided is not a valid trace record." << std::endl;
std::cerr << "OFFENDING LINE: " << test_string << std::endl;

return false;
}
// The given value for the field position isn't n_pos, so verify that the thing at that position is a number.
else
{
}
}
else
{
    trace_file.close();
    return false;
}
}
*/


