/*
 *  File Name : svm_module.cpp
 *  
 *  Creation Date : 01-31-2017
 *
 *  Last Modified : Tue 31 Jan 2017 06:26:38 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "svm_module.h"

SVM_Module::SVM_Module(){

    // TODO: make sure the constructor is sufficient
    // with nothing in it.
}


SVM_Module::~SVM_Module(){

    // TODO: Decide what the destructor should do
}

void SVM_Module::add_training_vector( double label, struct svm_node * node ){

    _class_labels.push_back( label );
    _support_vectors.push_back( node );
}

// CHECK:  This is a pretty sloppy way to handle this.
// Another solution would be to return a bool; true if
// a valid solution is found, false if the model is null.
// then, pass in a reference to a double.  In the 
// "true" case, set that double's value to the result of 
// svm_predict.

double SVM_Module::predict( const struct svm_node * node ){

    if ( _model != NULL )
    {
        return svm_predict( _model, node );
    }
    else
    {
        return 0.0;   //CHECK: This isn't ideal.
    }
}

int_fast32_t SVM_Module::save_model( const char * file_name ){

    return svm_save_model ( file_name, _model );
}

int_fast32_t SVM_Module::save_model( const std::string file_name ){

    return save_model( file_name.c_str() );
}

struct svm_model * load_model( const char * file_name ){

    return svm_load_model ( file_name );
}

struct svm_model * load_model( const std::string file_name ){

    return load_model ( file_name.c_str() );
}


