/*
 *  File Name : syscall_detector.h
 *  
 *  Creation Date : 04-10-2016
 *
 *  Last Modified : Tue 15 Nov 2016 02:10:01 AM EST
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

/*
 * This would be a good place to perhaps define the table sizes, starts, and ending points for various instruction sets.
 *
 * Citation: https://w3challs.com/syscalls/
 * 
 * I'll start here with MIPS.
 *
 * ================================
 *
 * Linux Mips o32
 *
 * Start: 4000 (not implemented, really starts at 4001)
 * End: 4346
 * Size: 346 entries
 *
 * --------------
 *
 * Linux Mips n32
 *
 * Start: 6000
 * End: 6310
 * Size: 310 entries
 *
 * --------------
 *
 * Linux Mips n64
 *
 * Start: 5000
 * End: 5305
 * Size: 305 entries
 *
 * =================================
 *
 * Linux ARM (aka arm_strong)
 *
 * Start: 9437184
 * End: 9437561
 * Size: 345 entries (the spread is really 377, not all numbers are implemented)
 *
 * --------------
 *
 * Linux ARM (thumb mode, aka arm_thumb)
 *
 * Start: 0
 * End: 377
 * Size: 345 (the spread is 377, not all numbers are implemented)
 * NOTE: 983045 sometimes appears.  I can't seem to figure out why...
 *
 * ==================================
 *
 * Linux x86
 *
 * Start: 0
 * End: 349
 * Size: 346 (the spread is 349)
 *
 * --------------
 *
 * Linux x86_64
 *
 * Start: 0
 * End: 312
 * Size: 313
 * 
 */

class Syscall_Detector : public Sensor_Observer {

    // TODO: Write a VERY BASIC detector.  You'll have to get the libsvm code.
}
