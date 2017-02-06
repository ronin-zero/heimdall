/*
 *  File Name : svm_module.h
 *  
 *  Creation Date : 01-25-2017
 *
 *  Last Modified : Sun 05 Feb 2017 08:04:01 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <cstring>

#include "libsvm/svm.h"

class SVM_Module{

    public:

        SVM_Module();
        virtual ~SVM_Module();

        virtual void add_training_vector( double label, struct svm_node * node ) = 0;

        virtual bool predict( const struct svm_node * node, double & label ) = 0;

        int_fast32_t save_model( const char * file_name );
        int_fast32_t save_model( const std::string file_name );

        struct svm_model * load_model( const char * file_name );
        struct svm_model * load_model( const std::string file_name );

    protected:

        bool trained;

        struct svm_model * _model;
        std::vector<struct svm_node *> _support_vectors;

        struct svm_parameter * _parameters;

        uint_fast32_t _problem_size;
        std::vector<double> _class_labels;

};
