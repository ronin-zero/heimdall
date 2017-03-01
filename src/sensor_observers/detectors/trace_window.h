/*
 *  File Name : trace_window.h
 *  
 *  Creation Date : 08-01-2017
 *
 *  Last Modified : Tue 28 Feb 2017 05:59:14 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstddef>
#include <deque>

using std::string;
using std::ostream;

class Trace_Window{

    public:

        Trace_Window( size_t trace_window_length );
        ~Trace_Window();

        int_fast32_t at( size_t index );

        size_t get_trace_length();
        size_t get_index();

        bool trace_window_full();

        bool add_data_point( int_fast32_t data_point );

        void reset_window();

        friend ostream& operator<<( ostream& s_out, Trace_Window const& window );

        const int_fast32_t& operator[]( uint_fast32_t i ) const;

    private:

        int_fast32_t* _trace_window;

        size_t _trace_length;
        size_t _index;

        void print( ostream& s_out ) const;
};
