/*
 *  File Name : output_stream.h
 *  
 *  Creation Date : 31-05-2016
 *
 *  Last Modified : Sat 05 Nov 2016 02:47:53 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <stdint.h>
#include <mutex>

#include "data_stream.h"
#include "sensor_observers/data_records/data_record.h"

using std::string;
using std::ostream;

class Output_Stream : public Data_Stream{

    public:

        // Constructors
        
        Output_Stream( uint_fast8_t out_flags=ALL, string sep=","); // Default
        Output_Stream( ostream& o_stream, uint_fast8_t out_flags=ALL, string sep="," ); // Alternate that takes an existing ostream reference.
        Output_Stream( string file_name, uint_fast8_t out_flags=ALL, string sep="," ); // Takes a string.  Will open a file output stream.

        ~Output_Stream();

        void process_data( Data_Record& record );

        void set_flags( uint_fast8_t new_flags );
        uint_fast8_t get_flags();

        void set_separator( string sep );
        string get_separator();

    private:

        bool needs_delete;  // The constructor that takes a string as an argument allocates memory.  It sets this value to true.
                            // The deconstructor checks if this is true and if it is, deletes "out."

        string separator;
        uint_fast8_t flags;

        ostream* out;
};
