/*
 *  File Name : regex_stream.h
 *  
 *  Creation Date : 11-17-2018
 *
 *  Last Modified : Sat 17 Nov 2018 05:36:41 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <mutex>
#include <regex>

#include "data_stream.h"
#include "sensor_data/data_patterns/syscall_patterns/linux_syscall_constants.h"
#include "sensor_observers/data_records/data_record.h"
#include "sensor_observers/data_records/linux/linux_syscall_record.h"

using std::string;
using std::ostream;

 /* NOTE: THIS CLASS IS JUST TO TEST REGEX PERFORMANCE
  * FOR LOGGING.  THAT'S WHY IT'S SO SLOPPY.  IT SHOULD
  * PROBABLY BE REMOVED ENTIRELY.  THANK YOU.
  */

class Regex_Stream : public Data_Stream{

    public:

        // Constructors

        Regex_Stream( uint_fast8_t out_flags=ALL, string sep="," ); // Default
        Regex_Stream( ostream& o_stream, uint_fast8_t out_flags=ALL, string sep="," ); // Alternate that takes an existing ostream reference.
        Regex_Stream( string file_name, uint_fast8_t out_flags=ALL, string sep="," );  // Takes a string.  Will open a file output stream.

        ~Regex_Stream();

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

        std::regex syscall_regex;
        std::smatch syscall_matches;

        ostream* out;
};
