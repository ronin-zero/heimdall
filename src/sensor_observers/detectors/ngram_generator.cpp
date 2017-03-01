/*
 *  File Name : ngram_generator.cpp
 *  
 *  Creation Date : 01-11-2017
 *
 *  Last Modified : Wed 01 Mar 2017 01:43:03 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "ngram_generator.h"

NGram_Generator::NGram_Generator( uint_fast32_t n_value, uint_fast32_t table_size ){

    _n = n_value;

    _table_size = table_size;
}

/*
 * OK, so this requires some explanation.  Our features are frequences of ngrams of system calls.  If there are c
 * calls on a table and we are using 2-grams, the resulting table of ngram frequencies would be c^2 in size.
 * It is unwieldly (perhaps even impossible) to give a two dimensional representation of this set of data to
 * libsvm, so this maps what would otherwise be a tuple into a single scalar value.
 *
 * For example, with 2-grams with the values ( 3, 2 ) and an instruction set with 4 possible calls, we would return a
 * scalar value of 14, which results from 3 * ( 4 ) + 2.  For a 3-gram with the values (3, 2, 1), we would get a 
 * scalar value of 75, which results from 3 * ( 16 ) + 2 * ( 4 ) + 1.
 *
 * Using variables n for the length of the ngram, a table size of c calls, g_i as the ith member of the ngram (0-indexed),
 * and s for the resulting scalar value, we use the following generalized formula:
 *
 *      s = g_0 * c ^ ( (n - 1) - 0 ) + g_1 * ( ( n - 1 ) - 1 )
 */

int_fast64_t NGram_Generator::generate_data_point( Trace_Window trace, uint_fast32_t index ){

    int_fast64_t ngram_value = 0;

    for ( uint_fast32_t i = 0; i < _n; i++ )
    {
        ngram_value += ( int_fast64_t ) trace[ index + i ] * pow( _table_size, ( _n - 1 ) - i ); 
    }
    
    return ngram_value;
}

uint_fast32_t NGram_Generator::ngram_size(){

    return _n;
}

bool NGram_Generator::has_next( Trace_Window trace, uint_fast32_t index ){
    
    return ( trace.size() > ( index + _n ) );
}
