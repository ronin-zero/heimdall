/*
 *  File Name : svm_module.h
 *  
 *  Creation Date : 01-25-2017
 *
 *  Last Modified : Wed 08 Feb 2017 10:03:59 PM EST
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
#include "stopping_criteria/stopping_criterion.h"

class SVM_Module{

    public:

        //SVM_Module();
        virtual ~SVM_Module() = 0;

        virtual bool add_training_vector( double label, struct svm_node * node ) = 0;

        virtual bool predict( const struct svm_node * node, double & label ) = 0;

        virtual bool is_trained() = 0;

        int_fast32_t save_model( const char * file_name );
        int_fast32_t save_model( const std::string file_name );

        struct svm_model * load_model( const char * file_name );
        struct svm_model * load_model( const std::string file_name );

    protected:

        virtual bool generate_model() = 0;

        struct svm_model * _model;
        struct svm_parameter * _parameters;
        Stopping_Criterion * _stopping_criterion;

        bool trained;

        uint_fast32_t _training_samples;

        std::vector<struct svm_node *> _support_vectors;


        uint_fast32_t _problem_size;
        std::vector<double> _class_labels;

};
