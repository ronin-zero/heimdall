/*
 *  File Name : linux_syscall_record.h
 *  
 *  Creation Date : 26-05-2016
 *
 *  Last Modified : Thu 26 May 2016 03:25:55 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */
#pragma once

#include <string>

using std::string;

#include "data_records/data_record.h"

// Each of these represents a field
// in a line in trace_pipe.
// For now, 1 is not used.

const uint_fast8_t PROCESS_NAME = 0x80;
const uint_fast8_t PID          = 0x40;
const uint_fast8_t CPU          = 0x20;
const uint_fast8_t TRACE_FLAGS  = 0x10;
const uint_fast8_t TIMESTAMP    = 0x08;
const uint_fast8_t SYSCALL_NUM  = 0x04;
const uint_fast8_t SYSCALL_ARGS = 0x02;

/*
 * At the start of the file "trace"
 * (located at: /sys/kernel/debug/tracing/trace)
 * are 11 lines which appear (more-or-less) as follows.
 * The final line includes 9 '|' characters.  The
 * penultimate line labels each of these with the field
 * of the system call trace that they point to, except
 * for the 4th, 5th, 6th, and 7th '|' which are labeled
 * on lines 5, 6, 7, and 8, respectively (follow the
 * line created by '|', '/', '-' characters to the terminating
 * "=>" for the corresponding label).
 *
 * Some fields (like the timestamp) will change width
 * under certain circumstances (eg. the time requires
 * more digits to display), so we can't treat the index
 * at which the '|' character as found as the start, end,
 * or any consistent offset into the field.
 *
 * However, we CAN be sure that the character at the
 * corresponding index belongs to the field indicated.
 *
 * With that information, we can parse each record by field.
 *
 * For my own convenience, I have labeled their indices
 * after the last line of the example.  I index them
 * beginning at 0.
 *
 * # tracer: nop
 * #
 * # entries-in-buffer/entries-written: 0/15256649   #P:16
 * #
 * #                              _-----=> irqs-off
 * #                             / _----=> need-resched
 * #                            | / _---=> hardirq/softirq
 * #                            || / _--=> preempt-depth
 * #                            ||| /     delay
 * #           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
 * #              | |       |   ||||       |         |
 *               15 17     25  29..32     40        50
 *
 *  They are defined as constants below.
 *
 *  An example of a record found in trace_pipe looks like this:
 *
 *             bash-15509 [002] .... 112170.687012: sys_enter: NR 16 (0, 5403, 7ffd0c9241f0, 0, 0, 700600)
 *
 *  The system call number appears 2 spaces after the function name and the argument list is one space beyond that.
 */

const uint_fast8_t TASK_INDEX       = 15;
const uint_fast8_t PID_INDEX        = 17;
const uint_fast8_t CPU_INDEX        = 25;
const uint_fast8_t TFLAGS_INDEX     = 29;
const uint_fast8_t TIMESTAMP_INDEX  = 40;
const uint_fast8_t FUNCTION_INDEX  = 50;

class Linux_Syscall_Record:public Data_Record{

    public:
        Linux_Syscall_Record( Sensor_Data data, uint_fast8_t settings_flags=ALL );

        string raw_data;

        // Methods to get individual fields
        //
        // This just returns them as strings.

        string get_task();
        string get_pid();
        string get_cpu();
        string get_flags();
        string get_timestamp();
        string get_syscall();
        string get_syscall_args();

        // It might be handy to get some of the numeric fields as
        // numeric types:

        uint_fast64_t get_pid_num();
        uint_fast16_t get_syscall_num(); // The value will never be negative and 16 bits is more than enough for any current linux instruction set.

        float get_timestamp_num();  // In the interest of full disclosure, I do not have a compelling reason for choosing a float over a double.
                                    // There may come a time when it becomes clear that a float is not enough.
                                    // An indicator of this would be negative timestamp values.
        
        uint_fast8_t check_flags(); // This should return the flags data member, not the flags portion of the raw_data.

        // TODO:
        // 
        // There is currently no implementation file for this header.
        // Do that, then make sure everything still compiles.


    protected:

        uint_fast8_t flags;
        void print( std::ostream& s_out ) const;

};
