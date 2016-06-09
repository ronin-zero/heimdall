/*
 *  File Name : ascii_operations.cxx
 *  
 *  Creation Date : 01-06-2016
 *
 *  Last Modified : Fri 03 Jun 2016 09:20:33 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "utils/ascii_operations.h"

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

uint_fast8_t ASCII_Operations::char_val( char num ){
    
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

    uint_fast64_t sign = 1;  // We'll multiply the result by this.  It will be -1 if the first char is '-'

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

bool ASCII_Operations::is_num( char c ){

    return ( c >= INT_RANGE_START && c <= INT_RANGE_END );
}

bool ASCII_Operations::is_caps( char c ){

    return ( c >= CAPS_RANGE_START && c <= CAPS_RANGE_END );
}

bool ASCII_Operations::is_lower( char c ){

    return ( c >= LOWER_RANGE_START && c <= LOWER_RANGE_END );
}

bool ASCII_Operations::is_alpha( char c ){

    return ( is_lower( c ) || is_caps( c ) );
}

bool ASCII_Operations::is_alphanum( char c ){

    return ( is_alpha( c ) || is_num( c ) );
}
