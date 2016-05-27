#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>

using std::string;

#include "sensor_data/sensor_data.h"

const uint_fast8_t ALL = 0xFF;

class Data_Record{

    public:

        // For now, default to all of the flags being on.
        virtual Data_Record( Sensor_Data data, uint_fast8_t setting_flags=ALL ) = 0;
        ~Data_Record();

        virtual string raw_string() = 0;

        // TODO: I want to try making this protected.  If it doesn't work, fall back to public.
        // virtual void print( std::ostream& s_out ) const = 0;
        // 
        // Below is the "friend" declaration of the << operator.  I think this should be OK.
        friend std::ostream& operator<<( std::ostream& s_out, Data_Record const& record );

        //TODO: make this interface
        // virtual void print( const ostream & stream ) = 0;
    
    protected:
        
        uint_fast8_t flags;
        string raw_data;
        virtual void print( std::ostream& s_out ) const = 0;



};

friend std::ostream& operator<<( std::ostream& s_out, Data_Record const& record ){

        record.print( s_out );
        return s_out;
}
