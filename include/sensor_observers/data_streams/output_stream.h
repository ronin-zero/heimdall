/*
 *  File Name : output_stream.h
 *  
 *  Creation Date : 31-05-2016
 *
 *  Last Modified : Tue 31 May 2016 10:28:04 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <fstream>
#include <ostream>
#include <string>

#include "sensor_observers/data_streams/data_stream.h"
#include "sensor_observers/record_factory.h"
#include "sensor_observers/data_record.h"

using std::string;
using std::ostream;

class Output_Stream : public Data_Stream{

    public:

        // Constructors
        
        Output_Stream(); // Default
        Output_Stream( ostream& o_stream ); // Alternate that takes an existing ostream reference.
        Output_Stream( string file_name ); // Takes a string.  Will open a file output stream.

        void process_data( Sensor_Data record );

        uint_fast8_t set_flags( uint_fast8_t new_flags );

    private:

        ostream out;
        Record_Factory * factory;
};
