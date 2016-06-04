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

        // For now, default to all of the flags being on.
        // Data_Record( Sensor_Data data, uint_fast8_t setting_flags=ALL, char sep=',' ) ;
        virtual ~Data_Record() = 0;

        virtual string raw_string() = 0;

        // CHECK: I want to try making this protected.  If it doesn't work, fall back to public.
        // virtual void print( ostream& s_out ) const = 0;
        // 
        // Below is the "friend" declaration of the << operator.  I think this should be OK.
        friend ostream& operator<<( ostream& s_out, Data_Record const& record );
    
    protected:
        
        uint_fast8_t flags;
        string raw_data;
        string separator;

        virtual void print( ostream& s_out ) const = 0;
};

// This is here just to give a basic constructor.
// I don't like implementing functions in header files.
/*
Data_Record::Data_Record( Sensor_Data data, uint_fast8_t setting_flags, char sep ){

    raw_data = data.get_data();
    flags = setting_flags;
}*/

// This must be defined like this in order to have polymorphic behavior of <<

ostream& operator<<( ostream& s_out, Data_Record const& record ){

        record.print( s_out );
        return s_out;
}
