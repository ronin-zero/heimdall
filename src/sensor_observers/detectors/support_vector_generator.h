/*
 *  File Name : support_vector_generator.h
 *  
 *  Creation Date : 18-01-2017
 *
 *  Last Modified : Tue 21 Feb 2017 12:12:32 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

//#include <cstdio>
#include <map>
#include <vector>

#include "libsvm/svm.h"

using std::map;

class Support_Vector_Generator{

    public:

        Support_Vector_Generator( uint_fast32_t max_data_points );
        ~Support_Vector_Generator();

        void add_data_point( uint_fast64_t data_point );
        bool full();

        struct svm_node * get_support_vector();

        void reset();

        map<uint_fast64_t, uint_fast32_t> get_table();

    private:

        uint_fast32_t _max_data_points;
        uint_fast32_t _data_point_count;

        map<uint_fast64_t, uint_fast32_t> call_table;
};
