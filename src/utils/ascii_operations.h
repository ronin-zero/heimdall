/*
 *  File Name : ascii_operations.h
 *  
 *  Creation Date : 06-01-2016
 *
 *  Last Modified : Fri 30 Jun 2017 08:31:29 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

// This file contains code to perform operations
// such as converting strings to various numerical
// types.
//
// The idea is to make them faster than the usual
// operations and to give more control over the
// exact type of number returned (eg. uint_fast8_t vs
// int8_t vs uint64_t etc).

#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath> 

// This should be the same across platforms, but
// just in case, this gets us the integer values
// of the start and of the range of values for 
// chars representing numbers, the offset
// between chars representing numbers and the
// numbers they represent, and the offset
// between lowercase and capital letters.

static const uint_fast8_t INT_OFFSET        = '0';
static const uint_fast8_t INT_RANGE_START   = '0';
static const uint_fast8_t INT_RANGE_END     = '9';

static const uint_fast8_t CAPS_OFFSET       = 'a' - 'A';
static const uint_fast8_t CAPS_RANGE_START  = 'A';
static const uint_fast8_t CAPS_RANGE_END    = 'Z';

static const uint_fast8_t LOWER_RANGE_START = 'a';
static const uint_fast8_t LOWER_RANGE_END   = 'z';

static const uint_fast8_t LOWER_HEX_START   = 'a';
static const uint_fast8_t LOWER_HEX_END     = 'f';
static const uint_fast8_t LOWER_HEX_OFFSET  = 87;

static const uint_fast8_t CAPS_HEX_START    = 'A';
static const uint_fast8_t CAPS_HEX_END      = 'F';
static const uint_fast8_t CAPS_HEX_OFFSET   = 55;

// Note that a single digit cannot possibly have this integer value.

static const uint_fast8_t ERROR_VAL         = 0xFF;

class ASCII_Operations{

    public:

        // Value of individual char

        static uint_fast8_t char_val( uint_fast8_t num );

        // Only convert numbers within a range (unsigned)
        
        static uint_fast64_t convert_range( std::string num, uint_fast64_t start, uint_fast64_t chars );

        // Signed integral values

        static int_fast64_t to_int( std::string num );

        // Unsigned integral values

        static uint_fast64_t to_uint( std::string num );

        // Floating point values

        static double to_floating_point( std::string num );

        // Hexidecimal conversions

        static uint_fast8_t hex_byte_val( std::string num );
        static uint_fast8_t hex_digit_val( uint_fast8_t c );

        // range checking

        static bool is_num( uint_fast8_t c );
        static bool is_caps( uint_fast8_t c );
        static bool is_lower( uint_fast8_t c );
        static bool is_alpha( uint_fast8_t c );
        static bool is_alphanum( uint_fast8_t c );
        static bool is_hex_digit( uint_fast8_t c );

        static bool is_hex_byte( std::string input );
        static bool is_number( std::string input );
        static bool is_floating_point( std::string input );
};
