/*
 *  File Name : output_stream.h
 *  
 *  Creation Date : 31-05-2016
 *
 *  Last Modified : Mon 06 Jun 2016 07:16:52 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>

#include "sensor_observers/data_streams/data_stream.h"
#include "sensor_observers/data_records/record_factory.h"
#include "sensor_observers/data_records/data_record.h"

using std::string;
using std::ostream;

class Output_Stream : public Data_Stream{

    public:

        // Constructors
        
        Output_Stream( uint_fast8_t out_flags=ALL, string sep=","); // Default
        Output_Stream( ostream& o_stream, uint_fast8_t out_flags=ALL, string sep="," ); // Alternate that takes an existing ostream reference.
        Output_Stream( string file_name, uint_fast8_t out_flags=ALL, string sep="," ); // Takes a string.  Will open a file output stream.

        void process_data( Data_Record record );

        void set_flags( uint_fast8_t new_flags );
        uint_fast8_t get_flags();

        void set_separator( string sep );
        string get_separator();

    private:

        string separator;
        uint_fast8_t flags;

        ostream * out;
        Record_Factory * factory;
};