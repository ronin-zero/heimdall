/*
 *  File Name : syscall_detector.h
 *  
 *  Creation Date : 04-10-2016
 *
 *  Last Modified : Tue 27 Dec 2016 10:44:08 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include "sensor_data/sensor_data.h"
#include "sensor_observers/sensor_observer.h"
#include "sensor_observers/data_records/data_record.h"
#include "sensor_observers/data_records/system_call_record.h"

/*  
 *  For now, only Linux is supported.  If other operating systems are added later, handle 
 *  them similarly.
 */

#ifdef __linux__
#include "sensor_observers/data_records/linux/linux_syscall_record.h"
typedef Linux_Syscall_Record Syscall_Record;
#else
typedef System_Call_Record Syscall_Record;
#endif

#ifdef __arm__
#include "sensor_observers/detectors/syscall_formatters/arch/arm/arm_syscall_formatter.h"
typedef ARM_Syscall_Formatter System_Call_Formatter;
#elif __mips__
#include "sensor_observers/detectors/syscall_formatters/arch/mips/mips_syscall_formatter.h"
typedef MIPS_Syscall_Formatter System_Call_Formatter;
#else
#include "sensor_observers/detectors/syscall_formatters/syscall_formatter.h"
typedef Syscall_Formatter System_Call_Formatter;
#endif

class Syscall_Detector : public Sensor_Observer{

    public:

        Syscall_Detector();
        ~Syscall_Detector();

        void update();
        void update( Sensor_Data data );

        void set_observing( bool on );
        void set_processing( bool on );

        bool observing_status();
        bool processing_status();

        void start_observing();
        void start_processing();

        void stop_observing();
        void stop_processing();

    private:

        void process();

        Syscall_Formatter * call_formatter;

        //Syscall

};

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
 * That means the spread is really 378.
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
