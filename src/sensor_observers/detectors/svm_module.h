/*
 *  File Name : svm_module.h
 *  
 *  Creation Date : 25-01-2017
 *
 *  Last Modified : Fri 27 Jan 2017 02:59:36 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <vector>

#include "libsvm/svm.h"

class SVM_Module{

    public:

        SVM_Module();

        virtual void add_training_data( double label, struct svm_node * node )=0;

    protected:

        bool trained;

        struct svm_model * _model;
        vector<struct svm_node *> _support_vectors;

        struct svm_parameter * _parameters;

        uint_fast32_t _problem_size;
        vector<double> _class_labels;

};
