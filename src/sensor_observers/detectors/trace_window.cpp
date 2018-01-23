/*
 *  File Name : trace_window.cpp
 *  
 *  Creation Date : 01-08-2017
 *
 *  Last Modified : Tue 23 Jan 2018 03:35:58 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "trace_window.h"

Trace_Window::Trace_Window( size_t trace_window_length ) : _trace_window() {

    _trace_length = trace_window_length;
}

// Really nothing needed here.

Trace_Window::~Trace_Window(){

}

size_t Trace_Window::get_trace_length(){

    return _trace_length;
}

size_t Trace_Window::size(){

    return _trace_window.size();
}

bool Trace_Window::full(){

    return ( _trace_window.size() >= _trace_length );
}

void Trace_Window::add_data_point( int_fast32_t data_point ){

    if ( full() )
    {
        _trace_window.pop_front();
    }
    
    _trace_window.push_back( data_point );
}

void Trace_Window::pop_front(){

    _trace_window.pop_front();
}

void Trace_Window::clear(){

    _trace_window.clear();
}

std::ostream& operator<<( std::ostream& s_out, Trace_Window const& window ){

    window.print( s_out );
    return s_out;
}

const int_fast32_t& Trace_Window::operator[]( uint_fast32_t i ) const{

    return _trace_window[ i ];
}

void Trace_Window::print( std::ostream& s_out ) const{

    s_out << '[';

    if ( !_trace_window.empty() )
    {
        s_out << _trace_window[ 0 ];

        for ( uint_fast32_t i = 1; i < _trace_window.size(); i++ )
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
