/*
 *  File Name : svm_module.h
 *  
 *  Creation Date : 25-01-2017
 *
 *  Last Modified : Wed 25 Jan 2017 05:26:22 PM EST
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

        

        

    private:

        bool training;

        struct svm_model * _model;
        vector<svm_node *> _support_vectors;

        struct svm_parameter * _parameters;

        uint_fast32_t _problem_size;
        vector<double> _class_labels;

};
