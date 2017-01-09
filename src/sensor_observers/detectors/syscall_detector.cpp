/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 27-12-2016
 *
 *  Last Modified : Mon 09 Jan 2017 01:20:01 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "syscall_detector.h"

Syscall_Detector::Syscall_Detector(){

    observing = false;
    processing = false;

    call_formatter = new Syscall_Formatter();
}

Syscall_Detector::~Syscall_Detector(){

    delete ( call_formatter );
}

void Syscall_Detector::update(){

    // The default version is not needed at this time.
}

void Syscall_Detector::update( Sensor_Data data ){

    if ( observing )
    {
        data_queue.enqueue( data );
    }
}

bool Syscall_Detector::observing_status(){

    return observing;
}

bool Syscall_Detector::processing_status(){

    return processing;
}

void Syscall_Detector::start_observing(){

    // TODO: this method.
}

void Syscall_Detector::start_processing(){

    // TODO: this method as well.
}

void Syscall_Detector::stop_observing(){

}
