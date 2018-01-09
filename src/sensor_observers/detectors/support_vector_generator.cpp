/*
 *  File Name : support_vector_generator.cpp
 *  
 *  Creation Date : 01-18-2017
 *
 *  Last Modified : Tue 09 Jan 2018 03:23:41 AM EST
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

    return _data_point_count >= _max_data_points;
}

struct svm_node * Support_Vector_Generator::get_support_vector(){

    svm_node * node_instance = NULL;
    
    uint_fast32_t ngram_count = call_table.size();

    if ( ngram_count > 0 )
    {
        // FIXME: This is probably the cause of somem problems.  Look at svm-toy/qt/svm-toy.cpp line 239 at how to do this and check your pointers.
        node_instance = (svm_node *) malloc( sizeof( svm_node ) * ( ngram_count + 1 ) ); // The + 1 is to account for the last node at index -1.

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

// It is sometimes convenient to know how many data points the generator is meant to hold. This method achieves that.

uint_fast32_t Support_Vector_Generator::size() const{

    return _max_data_points;
}

uint_fast32_t Support_Vector_Generator::data_point_count() const {

    return _data_point_count;
}

uint_fast32_t Support_Vector_Generator::points_until_full() const {

    return _max_point_count - _data_point_count;
}

void Support_Vector_Generator::reset(){

    _data_point_count = 0;
    call_table.clear();
}

map<uint_fast64_t, uint_fast32_t> Support_Vector_Generator::get_table(){

    return call_table;
}
