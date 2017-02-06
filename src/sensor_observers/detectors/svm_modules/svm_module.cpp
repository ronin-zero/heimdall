/*
 *  File Name : svm_module.cpp
 *  
 *  Creation Date : 01-31-2017
 *
 *  Last Modified : Sun 05 Feb 2017 09:48:31 PM EST
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

// Originally, this returned a double.  I have changed it so that it takes a 
// reference to a double in addition to a pointer to an svm_node.  Now,
// if the model is NULL, it will return false and leave the value in the
// reference to the double untouched.  Otherwise, it will populate the
// reference with the label predicted according to the model for the given
// svm_node pointer input.

bool SVM_Module::predict( const struct svm_node * node, double & label ){

    if ( _model == NULL )
    {
        return false;
    }
    else
    {
        label = svm_predict( _model, node );
        return true;
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
