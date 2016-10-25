/*
 *  File Name : output_stream.cpp
 *  
 *  Creation Date : 06-06-2016
 *
 *  Last Modified : Sat 22 Oct 2016 07:13:43 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "output_stream.h"

// Debugging stuff...
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#define gettid() syscall(SYS_gettid)

Output_Stream::Output_Stream( uint_fast8_t out_flags, string sep ){

    flags = out_flags;
    separator = sep;

    out = &std::cout;

    needs_delete = false;
}

Output_Stream::Output_Stream( ostream & o_stream, uint_fast8_t out_flags, string sep ){

    flags = out_flags;
    separator = sep;

    out = &o_stream;

    needs_delete = false;
}

Output_Stream::Output_Stream( string file_name, uint_fast8_t out_flags, string sep ){

    flags = out_flags;
    separator = sep;
    
    out = new std::ofstream ( file_name );

    needs_delete = true;
}

// Destructor

Output_Stream::~Output_Stream(){

    if ( needs_delete )
    {
        delete ( out );
    }
}

// This function takes a pointer to a Data_Record so that
// applying the << operator (which calls print) will allow
// derived types of Data_Record to use their own appropriate
// extensions to this operator.

void Output_Stream::process_data ( Data_Record* record ){

    record->set_flags( flags );
    record->set_separator( separator );

    *out << *record << std::endl;
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
