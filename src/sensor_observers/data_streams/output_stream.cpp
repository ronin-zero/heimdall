/*
 *  File Name : output_stream.cpp
 *  
 *  Creation Date : 06-06-2016
 *
 *  Last Modified : Mon 13 Jun 2016 06:34:44 PM EDT
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

// Destructor

Output_Stream::~Output_Stream(){
    
    out->flush();
}

void Output_Stream::process_data ( Data_Record record ){

    *out << record;
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
