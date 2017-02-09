/*
 *  File Name : one_class_svm.h
 *  
 *  Creation Date : 02-08-2017
 *
 *  Last Modified : Wed 08 Feb 2017 06:59:55 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include "svm_module.h"

class One_Class_SVM: public SVM_Module{

    public:

        One_Class_SVM(); // TODO: Make constructors.
        
        ~One_Class_SVM();   // TODO: also make destructors.

        bool add_training_vector( double label=0.0, struct svm_node * node );

    private:

};
