/*
 *  File Name : trace_window.h
 *  
 *  Creation Date : 01-08-2017
 *
 *  Last Modified : Wed 01 Mar 2017 12:42:07 AM EST
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

class Trace_Window{

    public:

        Trace_Window( size_t trace_window_length );
        ~Trace_Window();

        size_t get_trace_length();
        size_t size();

        bool trace_window_full();

        bool add_data_point( int_fast32_t data_point );

        friend std::ostream& operator<<( std::ostream& s_out, Trace_Window const& window );

        const int_fast32_t& operator[]( uint_fast32_t i ) const;

    private:

        std::deque<int_fast32_t> _trace_window;

        size_t _trace_length;

        void print( std::ostream& s_out ) const;
};
