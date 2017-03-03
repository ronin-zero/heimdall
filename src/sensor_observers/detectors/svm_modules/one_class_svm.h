/*
 *  File Name : one_class_svm.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Thu 02 Mar 2017 03:09:27 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include "svm_module.h"

class One_Class_SVM: public SVM_Module{

    public:

        One_Class_SVM();

        One_Class_SVM( const char * file_name );
        One_Class_SVM( const std::string file_name );
        
        ~One_Class_SVM();

        void add_training_vector( struct svm_node * node, double label=0.0 );

        bool predict( const struct svm_node * node, double & label );

        //bool is_trained();

        bool load_model( const char * file_name );
        bool load_model( const std::string file_name );

        bool set_parameters( const struct svm_parameter * parameters );

    private:

        void set_default_parameters();

        bool generate_model();
};
