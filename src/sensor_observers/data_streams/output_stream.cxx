/*
 *  File Name : output_stream.cxx
 *  
 *  Creation Date : 06-06-2016
 *
 *  Last Modified : Mon 06 Jun 2016 07:19:14 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "sensor_observers/data_streams/output_stream.h"

Output_Stream::Output_Stream( uint_fast8_t out_flags, string sep ){

    flags = out_flags;
    separator = sep;

    out = &std::cout;
}

Output_Stream::Output_Stream( ostream & o_stream, uint_fast8_t out_flags, string sep ){

    flags = out_flags;
    separator = sep;

    out = &o_stream;
}

Output_Stream::Output_Stream( string file_name, uint_fast8_t out_flags, string sep ){

    flags = out_flags;
    separator = sep;

    std::ofstream fout( file_name );

    out = &fout;
}

void Output_Stream::set_flags( uint_fast8_t new_flags ){

    flags = new_flags;
}

uint_fast8_t Output_Stream::get_flags(){

    return flags;
}

void Output_Stream::set_separator( string sep ){

    separator = sep;
}

string Output_Stream::get_separator(){

    return separator;
}
