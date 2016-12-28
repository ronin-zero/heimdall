/*
 *  File Name : syscall_detector.cpp
 *  
 *  Creation Date : 27-12-2016
 *
 *  Last Modified : Tue 27 Dec 2016 10:44:21 PM EST
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
