/*
 *  File Name : support_vector_generator.cpp
 *  
 *  Creation Date : 18-01-2017
 *
 *  Last Modified : Wed 25 Jan 2017 12:52:23 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "support_vector_generator.h"

Support_Vector_Generator::Support_Vector_Generator( uint_fast32_t max_data_points ) : call_table(){

    _max_data_points = max_data_points;
    _data_point_count = 0;
}

Support_Vector_Generator::~Support_Vector_Generator(){

    call_table.clear();
}

void Support_Vector_Generator::add_data_point( uint_fast64_t data_point ){

    if ( call_table.count( data_point ) > 0 )
    {
        call_table[ data_point ]++;
    }
    else
    {
        call_table[ data_point ] = 1;
    }

    _data_point_count++;
}

bool Support_Vector_Generator::full(){

    return _max_data_points == _data_point_count;
}

svm_node * Support_Vector_Generator::get_support_vector(){

    svm_node * node_instance = NULL;
    
    uint_fast32_t ngram_count = call_table.size();

    if ( ngram_count > 0 )
    {
        node_instance = (svm_node *) malloc( sizeof( svm_node ) * ( ngram_count + 1 ) );

        uint_fast32_t i = 0;

        for ( std::map<uint_fast64_t, uint_fast32_t>::iterator it = call_table.begin(); it != call_table.end(); ++it )
        {
            node_instance[i].index = it->first;
            node_instance[i].value = it->second / (double) _data_point_count;

            i++;
        }

        // libsvm uses a sparse representation of matrices.
        // The end of svm_nodes for a row is signified by an index of -1.
        // The indices must be in ASCENDING ORDER.  The value of the last
        // node is unimportant.

        node_instance[i].index = -1;
        node_instance[i].value = 0.0;  // "Mr. Bl--  ... Mr. Blutarsky... Zero. Point. Zero.
    }

    return node_instance;
}

void Support_Vector_Generator::reset(){

    _data_point_count = 0;
    call_table.clear();
}

map<uint_fast64_t, uint_fast32_t> Support_Vector_Generator::get_table(){

    return call_table;
}
