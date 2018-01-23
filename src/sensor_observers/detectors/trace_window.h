/*
 *  File Name : trace_window.h
 *  
 *  Creation Date : 01-08-2017
 *
 *  Last Modified : Tue 23 Jan 2018 03:35:33 PM EST
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

        bool full();

        void add_data_point( int_fast32_t data_point );

        void pop_front();

        void clear();

        friend std::ostream& operator<<( std::ostream& s_out, Trace_Window const& window );

        const int_fast32_t& operator[]( uint_fast32_t i ) const;

    private:

        std::deque<int_fast32_t> _trace_window;

        size_t _trace_length;

        void print( std::ostream& s_out ) const;
};
