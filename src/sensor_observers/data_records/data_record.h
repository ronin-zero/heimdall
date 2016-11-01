#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

using std::string;
using std::ostream;

#include "sensor_data/sensor_data.h"

const uint_fast8_t ALL = 0xFF;

class Data_Record{

    public:

        Data_Record( const Sensor_Data& data, uint_fast8_t setting_flags=ALL, string sep="," );
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

        friend ostream& operator<<( ostream& s_out, Data_Record const& record );

    protected:
        
        uint_fast8_t flags;
        string raw_data;
        string separator;

        virtual void print( ostream& s_out ) const;
};
