/*
 *  File Name : one_class_svm.cpp
 *  
 *  Creation Date : 02-15-2017
 *
 *  Last Modified : Mon 12 Mar 2018 03:50:58 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "one_class_svm.h"

// Constructors

One_Class_SVM::One_Class_SVM(){

    _parameters = new svm_parameter();

    _training_samples = 0;
    _class_labels = new std::vector<double>();
    _support_vectors = new std::vector<struct svm_node*>();

    set_default_parameters();

    trained = false;
}

One_Class_SVM::One_Class_SVM( const char * file_name ) : One_Class_SVM() {

    trained = load_model( file_name );
}

One_Class_SVM::One_Class_SVM( const std::string file_name ) : One_Class_SVM( file_name.cstr() ) {

}

// Destructors

One_Class_SVM::~One_Class_SVM(){

    if ( trained )
    {
        svm_free_model_content( _model );
        free( _model );
    }
    else
    {
        SVM_Module::free_support_vectors();
    }

    delete _class_labels;
    delete _support_vectors;
    delete _parameters;
    
}

void One_Class_SVM::add_training_vector( struct svm_node * node, double label ){

    // CHECK: This seems like a sloppy way around the unused parameter warning, but 
    // I'm out of ideas.
    (void)label;
    return SVM_Module::add_training_vector( node, 0.0 );
}

bool One_Class_SVM::load_model( const char * file_name ){

    struct svm_model * temp_model = svm_load_model( file_name );

    // If load_model fails, this temp_model will be a null pointer.
    // If it succeeds and the resulting model's svm_type is not ONE_CLASS,
    // it can't be used as a model for one_class_svm (for obvious reasons)
    // and we should consider it a failure.

    if ( temp_model == NULL )
    {
        return false;
    }
    else if ( temp_model->param.svm_type != ONE_CLASS )
    {
        // Memory was allocated as a result of the successful load_model function,
        // but the svm type is incompatible.  The pointer must be freed before
        // the function returns false.

        free ( temp_model );
        return false;
    }

    // If the method hasn't returned false yet, temp_model is both non-NULL and
    // has svm_type ONE_CLASS.  Set _model to temp_model, set trained to true,
    // and return trained.

    _model = temp_model;
    trained = true;
    _training_samples = _model->l;
    set_parameters( &(_model->param) );

    return trained;
}

bool One_Class_SVM::load_model( const std::string file_name ){

    return load_model( file_name.c_str() );
}

bool One_Class_SVM::is_trained(){

    return trained;
}

// This effectively produces a deep copy of another svm_parameter into this class's _parameter member.

bool One_Class_SVM::set_parameters( const struct svm_parameter * parameters ){

    if ( parameters != NULL && parameters->svm_type == ONE_CLASS )
    {
        _parameters->svm_type       = parameters->svm_type;
        _parameters->kernel_type    = parameters->kernel_type;
        _parameters->degree         = parameters->degree;
        _parameters->gamma          = parameters->gamma;
        _parameters->coef0          = parameters->coef0;

        _parameters->cache_size     = parameters->cache_size;
        _parameters->eps            = parameters->eps;
        _parameters->C              = parameters->C;
        _parameters->nr_weight      = parameters->nr_weight;
        _parameters->weight_label   = parameters->weight_label;
        _parameters->nu             = parameters->nu;
        _parameters->p              = parameters->p;
        _parameters->shrinking      = parameters->shrinking;
        _parameters->probability    = parameters->probability;

        return true;
    }

    return false;
}

// TODO: Put the magic numbers in the method below into meaningful constants.

/* In order to avoid complications, the default parameters for a one_class_svm object
 * will be set to the following values:
 *
 * int svm_type = ONE_CLASS;    -This is the one_class_svm svm_module class. 
 * int kernel_type = LINEAR;    -The default kernel_type will be linear for now in the interest of simplicity.
 * int degree = 1;              -It's a LINEAR kernel_type, so this value actually doesn't matter and this is just for redundancy should the kernel_type change to poly later.
 * double gamma = 0.0;          -This is for poly/rbf/sigmoid kernels, so it's unused, and Bander has it set to 0 in his code (this is arbitrary).
 * double coef0 = 0.0;          -Like the value for gamma, this is unused when kernel_type is LINEAR (it is only used for poly/sigmoid).  Likewise, Bander's code has it set to 0.
 *
 * double cache_size = 100.0;   -100 is the default value (in MB).  This may be too large for some embedded systems, so CHECK this.
 * double eps = 1e-3;           -Full disclosure: This is what Bander used and my decision here is arbitrary.
 * double C = 1.0;              -The documentation on this is bad, but it's only used for svm_types C_SVC, EPSILON_SVR, and NU_SVR (also Bander has it set to 1).
 * int nr_weight = 0;           -This is used in C_SVC, so it will remain 0 as in Bander's code.
 * int *weight_lable = NULL;    -This is used in C_SVC, so it will remain NULL as in Bander's code.
 * double* weight = NULL;       -This is used in C_SVC, so it will remain NULL as in Bander's code.
 * double nu = 0.005;           -Again, this is taken from Bander's code.  NU_SVC, ONE_CLASS, and NU_SVR require a nu value.  CHECK as this is arbitrary (more or less).
 * double p = 0.1;              -This is used for EPSILON_SVR, not ONE_CLASS, so its value is the same as in Bander's code.
 * int shrinking = 1;           -CHECK: I got this from Bander's code as well, but I'm not sure I fully understand the meaning of this parameter.
 * int probability = 0;         -CHECK: Once again, I am using the value from Bander's code.
 */

void One_Class_SVM::set_default_parameters(){

    if ( _parameters != NULL )
    {
        _parameters->svm_type = ONE_CLASS;
        _parameters->kernel_type = LINEAR;
        _parameters->degree = 1;
        _parameters->gamma = 0.0;
        _parameters->coef0 = 0.0;

        _parameters->cache_size = 100.0;
        _parameters->eps = 1e-3;
        _parameters->C = 1.0;
        _parameters->nr_weight = 0;
        _parameters->weight_label = NULL;
        _parameters->weight = NULL;
        _parameters->nu = 0.005;
        _parameters->p = 0.1;
        _parameters->shrinking = 1;
        _parameters->probability = 0;
    }
}
