/*
 *  File Name : data_record.h
 *  
 *  Creation Date : 05-20-2016
 *
 *  Last Modified : Sat 08 Dec 2018 12:41:19 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <regex>

using std::string;
using std::ostream;

#include "sensor_data/sensor_data.h"

const uint_fast8_t ALL = 0xFF;

class Data_Record{

    public:

        Data_Record( const std::smatch matches, uint_fast8_t settings_flags=ALL, std::string sep="," );
        virtual ~Data_Record() {}

        std::string raw_string() const;
        void set_flags( uint_fast8_t new_flags );
        uint_fast8_t get_flags();
        void set_separator( std::string sep );
        std::string get_separator();

        // CHECK: I want to try making this protected.  If it doesn't work, fall back to public.
        // virtual void print( ostream& s_out ) const = 0;
        //
        // UPDATE: OK, print will be virtual, but not pure virtual.  That way, there can be a basic
        // functionality, but subclasses can redefine it.
        // 
        // Below is the "friend" declaration of the << operator.  I think this should be OK.

        // CHECK: Make sure that the const correctness is done right on this function and the print function
        friend std::ostream& operator<<( std::ostream& s_out, Data_Record const& record );

    protected:
        
        uint_fast8_t flags;
        std::smatch record_fields;
        std::string separator;

        virtual void print( std::ostream& s_out ) const;
};
