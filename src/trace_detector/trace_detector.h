/*
 *  File Name : trace_detector.h
 *  
 *  Creation Date : 06-16-2017
 *
 *  Last Modified : Tue 20 Jun 2017 06:43:28 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "utils/directory_handler.h"
#include "sensor_observers/detectors/trace_reader.h"
#include "sensor_observers/detectors/trace_window.h"
#include "sensor_observers/detectors/ngram_generator.h"
#include "sensor_observers/detectors/svm_modules/svm_module.h"
#include "sensor_observers/detectors/svm_modules/one_class_svm.h"
#include "sensor_observers/detectors/support_vector_generator.h"
#include "sensor_observers/detectors/linux/syscall_formatters/syscall_formatter.h"

class Trace_Detector {

    public:

        Trace_Detector( Syscall_Formatter * formatter, size_t window_size, uint_fast32_t ngram_length );
        ~Trace_Detector();

        void read_file_list( std::vector<std::string> files, uint_fast8_t sep=',' );
        bool train_from_trace( const std::string file_name, uint_fast8_t sep=',' );

        bool train_model();
        
    private:

        std::vector<std::string> processed_files;

        Syscall_Formatter * _call_formatter;
        Trace_Window _window;
        NGram_Generator _ngram_generator;
        Support_Vector_Generator _sv_generator;
        One_Class_SVM _one_class_svm;
};
