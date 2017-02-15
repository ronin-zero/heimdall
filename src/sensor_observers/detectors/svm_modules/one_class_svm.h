/*
 *  File Name : one_class_svm.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 15 Feb 2017 03:51:59 AM EST
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

        //bool add_training_vector( double label=0.0, struct svm_node * node );
        
        bool add_training_vector( struct svm_node * node );

        bool predict( const struct svm_node * node, double & label );

        //bool is_trained();

        bool load_model( const char * file_name );
        bool load_model( const std::string file_name );

    private:

        bool generate_model();
};
