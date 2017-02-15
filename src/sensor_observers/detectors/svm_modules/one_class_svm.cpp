/*
 *  File Name : one_class_svm.cpp
 *  
 *  Creation Date : 02-15-2017
 *
 *  Last Modified : Wed 15 Feb 2017 03:52:18 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "one_class_svm.h"

One_Class_SVM::One_Class_SVM(){

    // TODO: Make this constructor do some things.
}

One_Class_SVM::One_Class_SVM( const char * file_name ){

    // TODO: Load the file name listed, check if it worked.    
}

One_Class_SVM::One_Class_SVM( const std::string file_name ){

    // TODO: Same as the constructor that takes a const char *, but converts it to c_str.
}

bool One_Class_SVM::add_training_vector( struct svm_node * node ){

    return SVM_Module::add_training_vector( 0.0, node );
}

bool One_Class_SVM::load_model( const char * file_name ){

    _model = svm_load_model( file_name );

    if ( _model->param.kernel_type != ONE_CLASS )
    {
        _model = NULL;
    }

    return ( _model != NULL );
}

bool One_Class_SVM::load_model( const std::string file_name ){

    return load_model( file_name.c_str() );
}
