/*
 *  File Name : support_vector_generator.cpp
 *  
 *  Creation Date : 18-01-2017
 *
 *  Last Modified : Wed 18 Jan 2017 03:25:42 AM EST
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

map<uint_fast64_t, uint_fast32_t> Support_Vector_Generator::get_table(){

    return call_table;
}
