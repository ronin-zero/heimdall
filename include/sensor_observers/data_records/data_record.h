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
        Data_Record( Sensor_Data data, uint_fast8_t setting_flags=ALL, string sep="," ) ;

        string raw_string() const;

        void set_flags( uint_fast8_t new_flags );
        uint_fast8_t get_flags();

        void set_separator( string sep );
        string get_separator();

        // CHECK: I want to try making this protected.  If it doesn't work, fall back to public.
        // virtual void print( ostream& s_out ) const = 0;
        // 
        // Below is the "friend" declaration of the << operator.  I think this should be OK.

        friend ostream& operator<<( ostream& s_out, Data_Record const& record );
    
    protected:
        
        uint_fast8_t flags;
        string raw_data;
        string separator;

        void print( ostream& s_out ) const;
};

// This is here just to give a basic constructor.
// I don't like implementing functions in header files.

Data_Record::Data_Record( Sensor_Data data, uint_fast8_t setting_flags, string sep ){

    raw_data = data.get_data();
    flags = setting_flags;
    separator = sep;
}

string Data_Record::raw_string() const{

    return raw_data;
}

void Data_Record::set_flags( uint_fast8_t new_flags ){

    flags = new_flags;
}

uint_fast8_t Data_Record::get_flags(){

    return flags;
}

void Data_Record::set_separator( string sep ){

    separator = sep;
}

string Data_Record::get_separator(){

    return separator;
}

// This must be defined like this in order to have polymorphic behavior of <<

ostream& operator<<( ostream& s_out, Data_Record const& record ){

        record.print( s_out );
        return s_out;
}

// This is to give default behavior if there isn't a 
// type of record available for that type of data/os.
// It just prints out the raw data. 

void Data_Record::print( ostream& s_out ) const{

    s_out << raw_string();
}
