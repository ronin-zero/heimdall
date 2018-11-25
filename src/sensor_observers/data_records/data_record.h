/*
 *  File Name : data_record.h
 *  
 *  Creation Date : 05-20-2016
 *
 *  Last Modified : Sun 25 Nov 2018 12:38:16 AM EST
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

        Data_Record( const Sensor_Data& data, uint_fast8_t settings_flags=ALL, std::string sep="," );
        Data_Record( const std::smatch& matches, uint_fast8_t settings_flags=ALL, std::string sep="," );
        virtual ~Data_Record() {}

        string raw_string() const;
        void set_flags( uint_fast8_t new_flags );
        uint_fast8_t get_flags();
        void set_separator( string sep );
        string get_separator();

        // CHECK: I want to try making this protected.  If it doesn't work, fall back to public.
        // virtual void print( ostream& s_out ) const = 0;
        //
        // UPDATE: OK, print will be virtual, but not pure virtual.  That way, there can be a basic
        // functionality, but subclasses can redefine it.
        // 
        // Below is the "friend" declaration of the << operator.  I think this should be OK.

        // CHECK: Make sure that the const correctness is done right on this function and the print function
        friend ostream& operator<<( ostream& s_out, Data_Record const& record );

    protected:
        
        uint_fast8_t flags;
        string raw_data;
        string separator;

        virtual void print( ostream& s_out ) const;
};
