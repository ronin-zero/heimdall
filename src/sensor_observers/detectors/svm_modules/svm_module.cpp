/*
 *  File Name : svm_module.cpp
 *  
 *  Creation Date : 01-31-2017
 *
 *  Last Modified : Wed 08 Feb 2017 10:20:00 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "svm_module.h"

// This method takes a label and a pointer to an svm_node struct (for training).
// It pushes the label onto the vector of training labels and the pointer to the
// svm_node struct onto the vector of svm_node struct pointers.  It increments
// the number of training samples by one, then consults the stopping_criteria
// to determine if the requirements have been met and the model can be generated.
// It returns the value of the _stopping_criterion pointer's can_stop_training()
// method, which tells the sensor using this svm_node that it can now generate
// a model and begin testing if so desired.

bool SVM_Module::add_training_vector( double label, struct svm_node * node ){

    _class_labels.push_back( label );
    _support_vectors.push_back( node );

    _training_samples++;

    return _stopping_criterion->can_stop_training();
}

// Originally, this returned a double.  I have changed it so that it takes a 
// reference to a double in addition to a pointer to an svm_node.  Now,
// if the model is NULL, it will return false and leave the value in the
// reference to the double untouched.  Otherwise, it will populate the
// reference with the label predicted according to the model for the given
// svm_node pointer input.

bool SVM_Module::predict( const struct svm_node * node, double & label ){

    if ( !trained || _model == NULL )
    {
        return false;
    }
    else
    {
        label = svm_predict( _model, node );
        return true;
    }
}

// The method generate_model will set trained to be true upon success.
// Also, loading a model via the load model methods will set it to true
// as will any other method that results in a valid model being created.

bool SVM_Module::is_trained(){

    return trained;
}

int_fast32_t SVM_Module::save_model( const char * file_name ){

    return svm_save_model ( file_name, _model );
}

int_fast32_t SVM_Module::save_model( const std::string file_name ){

    return save_model( file_name.c_str() );
}

// TODO:  These should be revised more and perhaps even left
// as pure abstract.  They should set "trained" to true.

struct svm_model * load_model( const char * file_name ){

    return svm_load_model ( file_name );
}

// TODO:  These should be revised more and perhaps even left
// as pure abstract.  They should set "trained" to true.

struct svm_model * load_model( const std::string file_name ){

    return load_model ( file_name.c_str() );
}
