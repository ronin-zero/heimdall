/*
 *  File Name : one_class_svm.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Sun 19 Feb 2017 04:51:40 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include "svm_module.h"

class One_Class_SVM: public SVM_Module{

    public:

        One_Class_SVM(); // TODO: Make constructors.

        One_Class_SVM( const char * file_name );
        One_Class_SVM( const std::string file_name );
        
        ~One_Class_SVM();   // TODO: also make destructors.

        bool add_training_vector( const struct svm_node * node, double label=0.0 );

        bool predict( const struct svm_node * node, double & label );

        //bool is_trained();

        bool load_model( const char * file_name );
        bool load_model( const std::string file_name );

        bool set_parameters( const struct svm_parameter * parameters );

    private:

        void set_default_parameters();

        bool generate_model();
};
