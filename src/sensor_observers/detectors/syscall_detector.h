/*
 *  File Name : syscall_detector.h
 *  
 *  Creation Date : 04-10-2016
 *
 *  Last Modified : Tue 04 Oct 2016 10:49:32 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include "sensor_observers/sensor_observer.h"
#include "data_records/data_record.h"

#ifdef __linux__
#include "data_records/linux/linux_syscall_record.h"
typedef Linux_Syscall_Record Syscall_Record;
#else
typedef Data_Record Syscall_Record;
#endif

class Syscall_Detector : public Sensor_Observer {

    // TODO: Write a VERY BASIC detector.  You'll have to get the libsvm code.
}
