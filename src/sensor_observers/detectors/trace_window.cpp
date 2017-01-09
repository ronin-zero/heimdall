/*
 *  File Name : trace_window.cpp
 *  
 *  Creation Date : 08-01-2017
 *
 *  Last Modified : Mon 09 Jan 2017 01:16:53 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "trace_window.h"

Trace_Window::Trace_Window( size_t trace_window_length ){

    _trace_length = trace_window_length;

    _trace_window[ _trace_length ];

    _index = 0;
}

Trace_Window::~Trace_Window(){

    // As of now, we don't need to do anything.
    // If I wind up allocating memory for the array representing the
    // trace window, of course I will have to free it here.
}

// This one should just provide "getter" access to the index provided by the parameter "index."
// It should almost certainly have some exception handling and bounds checking, but for now,
// only I'm using this, so I'm going to just trust myself to not provide malicious input.
//
// TODO: Exception handling and bounds checking here.
int_fast32_t Trace_Window::at( size_t index ){
    return _trace_window[ index ];
}

size_t Trace_Window::get_trace_length(){

    return _trace_length;
}

size_t Trace_Window::get_index(){

    return _index;
}

bool Trace_Window::trace_window_full(){

    return ( _index >= _trace_length );
}

bool Trace_Window::add_data_point( int_fast32_t data_point ){

    if ( !trace_window_full() )
    {
        _trace_window[ _index++ ] = data_point;
        return true;
    }
    else
    {
        return false;
    }
}

void Trace_Window::reset_window(){

    _index = 0;
}

ostream& operator<<( ostream& s_out, Trace_Window const& window ){

    window.print( s_out );
    return s_out;
}

void Trace_Window::print( ostream& s_out ) const{

    s_out << '[';

    if ( _index > 0 )
    {
        s_out << _trace_window[0];

        for ( uint_fast32_t i = 1; i < _index; i++ )
        {
            s_out << ',' << _trace_window[ i ];        
        }
    }
    else
    {
        s_out << "--EMPTY--";
    }

    s_out << ']';
}
