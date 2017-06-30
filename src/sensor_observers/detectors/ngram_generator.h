/*
 *  File Name : ngram_generator.h
 *  
 *  Creation Date : 01-11-2017
 *
 *  Last Modified : Fri 30 Jun 2017 05:58:19 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <cmath>

#include "data_point_generator.h"

class NGram_Generator : public Data_Point_Generator{

    public:

        NGram_Generator( uint_fast32_t n_value, uint_fast32_t table_size );
        ~NGram_Generator() {}

        int_fast64_t generate_data_point( Trace_Window& trace, uint_fast32_t index = 0 );

        uint_fast32_t ngram_size();

        bool has_next( Trace_Window& trace, uint_fast32_t index = 0 );

    private:

        uint_fast32_t _n;

        uint_fast32_t _table_size;
};
