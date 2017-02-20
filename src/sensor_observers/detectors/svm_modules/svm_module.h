/*
 *  File Name : svm_module.h
 *  
 *  Creation Date : 01-25-2017
 *
 *  Last Modified : Mon 20 Feb 2017 02:51:18 AM EST
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

        virtual ~SVM_Module() = 0;

        virtual bool add_training_vector( struct svm_node * node, double label ) = 0;

        virtual bool predict( struct svm_node * node, double & label ) = 0;

        virtual bool is_trained() = 0;
        
        virtual bool load_model( const char * file_name ) = 0;
        virtual bool load_model( const std::string file_name ) = 0;

        int_fast32_t save_model( const char * file_name );
        int_fast32_t save_model( const std::string file_name );

        virtual bool set_parameters( const struct svm_parameter * parameters ) = 0;

        virtual bool train_model() = 0;
        
    protected:

        virtual void set_default_parameters() = 0;

        virtual void make_problem() = 0;

        virtual bool generate_model() = 0;

        struct svm_problem * _problem;
        struct svm_model * _model;
        struct svm_parameter * _parameters;
        Stopping_Criterion * _stopping_criterion;

        bool trained;

        uint_fast32_t _training_samples;

        std::vector<struct svm_node *> _support_vectors;


        uint_fast32_t _problem_size;
        std::vector<double> _class_labels;

};
