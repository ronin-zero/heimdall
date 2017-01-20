/*
 *  File Name : support_vector_generator.cpp
 *  
 *  Creation Date : 18-01-2017
 *
 *  Last Modified : Fri 20 Jan 2017 06:31:34 AM EST
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

    // FIXME: this method is giving me problems.  I need to concentrate on it later.
    // It is imperative that it be fixed.
/*
    if ( call_table.size() > 0 )
    {
        node_instance = (svm_node *) malloc( sizeof( svm_node ) * ( call_table.size() + 1 ) );

        for ( uint_fast64_t i = 0, std::map<uint_fast64_t, uint_fast32_t>::iterator it = call_table.begin(); it != call_table.end(); ++it, i++ )
        {
            node_instance[i]->index = it->first;
            node_instance[i]->value = it->second / (double) _data_point_count;
        }
    }
*/
    return node_instance;
}

void Support_Vector_Generator::reset(){

    _data_point_count = 0;
    call_table.clear();
}

map<uint_fast64_t, uint_fast32_t> Support_Vector_Generator::get_table(){

    return call_table;
}
