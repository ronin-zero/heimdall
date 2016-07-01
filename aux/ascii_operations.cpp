/*
 *  File Name : ascii_operations.cpp
 *  
 *  Creation Date : 01-06-2016
 *
 *  Last Modified : Thu 30 Jun 2016 10:24:30 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "ascii_operations.h"

// CHECK:  This whole class implementation might be a bottleneck with
// the somewhat paranoid range checking and whatnot.

// CHECK:  I am making the assumption that, in lieu of a general int
// type that one would normally find in C++, I can just use 
// int_fast64_t and uint_fast64_t and the logic in stdint.h and/or
// the compiler optimizations will figure out which type I need
// by virtue of assignment rather than casting.  This could
// turn out to be a completely disasterous and stupid assumption.

/*
 *  Generally, this class contains functions that iterate over the
 *  individual characters of a string, subtract the offset of the
 *  ascii value and the value which it represents, adds them to a
 *  running sum that is multiplied by 10 prior to each addition
 *  (to move the decimal place over 1), and returns the sum once
 *  the entire range is read.  This is (reportedly: see citation
 *  below) faster than atoi or other standard methods, and time
 *  is a premium on embedded systems.  I'm hoping the use of "fast"
 *  types helps in this regard as well.
 *
 *  Signed types first check if the first character is '-'.  If it is,
 *  the conversion starts at the second character and the end result is
 *  multiplied by -1.
 *
 *  For floating point types (float, double), the string is split at the
 *  decimal place and each half is evaluated as a signed integer as
 *  described above.  The decimal portion is divided by 10^(n) where n 
 *  is the number of digits appearing after the decimal point.  Then,
 *  the two portions are added together and returned.
 */

uint_fast8_t ASCII_Operations::char_val( uint_fast8_t num ){
    
    return ( is_num( num ) ? num - INT_OFFSET : ERROR_VAL );
}

// num is the string containing the number to be converted.
// start is the index of the place where we'll start the conversion.
// chars is the number of digits in the number that we want to convert; how
// many characters to convert beginning with start.

uint_fast64_t ASCII_Operations::convert_range( string num, uint_fast64_t start, uint_fast64_t chars ){

    uint_fast64_t sum = 0;

    uint_fast64_t len = num.length();

    // Make sure that we don't go beyond the last char

    if ( start + chars > len )
    {
        chars = len - start;
    }

    for ( uint_fast64_t i = start; i < start + chars; i++ )
    {
        sum = sum * 10 + char_val( num[i] );
    }

    return sum;
}

int_fast64_t ASCII_Operations::to_int( string num ){

    uint_fast64_t string_val = 0;

    uint_fast64_t start_index = 0;
    uint_fast64_t len = num.length();

    uint_fast64_t sign = 1;  // We'll multiply the result by this.  It will be -1 if the first uint_fast8_t is '-'

    if ( num[0] == '-' )
    {
        start_index += 1;
        len -= 1;
        sign = -1;        
    }

    string_val = sign * convert_range ( num, start_index, len );

    return string_val;
}

uint_fast64_t ASCII_Operations::to_uint( string num ){
    
    uint_fast64_t string_val = convert_range( num, 0, num.length() );
   
    return string_val;
}

double ASCII_Operations::to_floating_point( string num ){

    double string_val = 0;

    string integer_portion;
    string decimal_portion;

    int_fast32_t integer_val = 0;
    double decimal_val = 0.0;

    uint_fast32_t int_digits = 0;
    uint_fast32_t dec_digits = 0;

    // uint_fast32_t len = num.length();
    int_digits = num.length();
    uint_fast32_t decimal_index = num.find('.');


    if ( decimal_index != string::npos )
    {
        decimal_portion = num.substr( decimal_index + 1 );

        dec_digits = decimal_portion.length();
        
        decimal_val = to_uint( decimal_portion ) / pow( 10, dec_digits );

        int_digits = decimal_index;
    }

    integer_portion = num.substr( 0, int_digits );

    integer_val = to_int( integer_portion );

    string_val = integer_val + decimal_val;
    
    return string_val;
}

uint_fast8_t ASCII_Operations::hex_byte_val( string num ){

    uint_fast8_t hex_val = 0;

    if ( is_hex_byte( num ) )
    {
        hex_val = hex_digit_val( num[2] );

        if ( num.length() > 3 )
        {
            hex_val = hex_val * 16 + hex_digit_val( num[3] );
        }
    }
    else
    {
        std::cerr << "ERROR: Argument is not a valid hexidecimal number." << std::endl;
        std::cerr << "Argument was: " << num << std::endl;
        std::cerr << "A hexidecimal value is of the form 0xNM where N and M are either numbers in the range 0-9, inclusive," << std::endl;
        std::cerr << "or letters in the ranges a-f or A-F, inclusive." << std::endl;
        std::cerr << "The return value of this function call will be " << ERROR_VAL << " (" << (uint_fast32_t)ERROR_VAL << "), but this" << std::endl;
        std::cerr << "is not an accurate value and should not be used." << std::endl;

        hex_val = ERROR_VAL;
    }

    return hex_val;
}

uint_fast8_t ASCII_Operations::hex_digit_val( uint_fast8_t c ){

    uint_fast8_t digit_val = 0;

    if ( is_hex_digit( c ) )
    {
        if ( is_num( c ) )
        {
            digit_val = c - INT_OFFSET; 
        }

        else if ( is_caps( c ) )
        {
            digit_val = c - CAPS_HEX_START;
        }

        else if ( is_lower( c ) )
        {
            digit_val = c - LOWER_HEX_START;
        }
        else
        {
            std::cerr << "ERROR: Argument is not a valid hex digit, but passed the check \"is_hex_digit( c )\"." << std::endl;
            std::cerr << "Argument was: " << c << std::endl;
            std::cerr << "Please report this bug to the code maintainers." << std::endl;

            digit_val = ERROR_VAL;
        }
    }
    else
    {
        std::cerr << "ERROR: Argument is not a valid hexidecimal digit." << std::endl;
        std::cerr << "Argument is: " << c << std::endl;
        std::cerr << "Valid hexidecimal digits are characters in the range 0-9, a-f, or A-F (inclusive for all)" << std::endl;
        std::cerr << "A value of " << ERROR_VAL << " (" << (uint_fast32_t)ERROR_VAL << ") will be returned, but this value should not be used." << std::endl;

        digit_val = ERROR_VAL;
    }

    return digit_val;
}

bool ASCII_Operations::is_num( uint_fast8_t c ){

    return ( c >= INT_RANGE_START && c <= INT_RANGE_END );
}

bool ASCII_Operations::is_caps( uint_fast8_t c ){

    return ( c >= CAPS_RANGE_START && c <= CAPS_RANGE_END );
}

bool ASCII_Operations::is_lower( uint_fast8_t c ){

    return ( c >= LOWER_RANGE_START && c <= LOWER_RANGE_END );
}

bool ASCII_Operations::is_alpha( uint_fast8_t c ){

    return ( is_lower( c ) || is_caps( c ) );
}

bool ASCII_Operations::is_alphanum( uint_fast8_t c ){

    return ( is_alpha( c ) || is_num( c ) );
}

bool ASCII_Operations::is_hex_digit( uint_fast8_t c ){

    return (    ( c >= INT_RANGE_START && c <= INT_RANGE_END ) ||
                ( c >= LOWER_HEX_START && c <= LOWER_HEX_END ) ||
                ( c >= CAPS_HEX_START && c <= CAPS_HEX_END )    );
}

bool ASCII_Operations::is_hex_byte( string input ){

    if ( input.length() > 4 || input.length() < 3 )
    {
        return false;
    }
    else if ( input[0] != '0' || ( input[1] != 'x' && input[1] != 'X' ) )
    {
        return false;
    }
    else
    {
        bool is_hex = is_hex_digit( input[2] );

        if ( is_hex && input.length() > 3 )
        {
            is_hex = is_hex_digit( input[3] );
        }

        return is_hex;
    }
}
