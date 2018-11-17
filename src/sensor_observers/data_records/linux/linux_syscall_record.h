/*
 *  File Name : linux_syscall_record.h
 *  
 *  Creation Date : 05-26-2016
 *
 *  Last Modified : Sat 17 Nov 2018 05:19:15 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <regex>

#include "sensor_observers/data_records/data_record.h"
#include "sensor_observers/data_records/system_call_record.h"
#include "sensor_data/data_patterns/syscall_patterns/linux_syscall_constants.h"

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
const uint_fast8_t FUNCTION_INDEX   = 50;

const uint_fast8_t NUM_TFLAGS       = 4; // This is just to avoid magic numbers.  There are 4 flags.

class Linux_Syscall_Record : public Data_Record, public System_Call_Record{

    public:
        
        Linux_Syscall_Record( const Sensor_Data& data, uint_fast8_t settings_flags=ALL, std::string sep="," );
        ~Linux_Syscall_Record() {}

        //string raw_string() const;

        // Methods to get individual fields
        //
        // This just returns them as strings.

        std::string get_task() const;
        std::string get_pid() const;
        std::string get_cpu() const;
        std::string get_trace_flags() const;
        std::string get_timestamp() const;
        std::string get_syscall() const;
        std::string get_syscall_args() const;

        // It might be handy to get some of the numeric fields as
        // numeric types:
        //
        // UPDATE: These functions are now inherited as pure virtual functions
        // from the System_Call_Record abstract class to allow generalized behavior across platforms.

        uint_fast32_t get_pid_num() const;
        uint_fast32_t get_syscall_num() const; // NOTE: Initially, I had this method return a uint_fast16_t reasoning that 16 bits was 
                                                // "more than enough" for any system call number regardless of instruction set.
                                                // This turns out not to be true.  For example, ARM Thumb-mode mostly has syscall numbers
                                                // ranging from 0-377, but there is a syscall that appears occasionally with the number
                                                // 983045.  Forcing this number into 16 bits results in a value of 5, which is already
                                                // in use. Regular ARM mode ("Strong") has system calls ranging from 0x900000 to 
                                                // 0x900179; none of its syscall numbers will fit in 16 bits.  Luckily, this method 
                                                // has not been used thus far, but will likely be needed for the detector.

        uint_fast16_t get_cpu_num() const;    // This will also be non-negative. As the CPU field is digits wide, the highest
                                        // possible value is 999 (theoretically).  8 bits is too few, but 16 bits is
                                        // more than enough to express this value.  It is unlikely that such a high
                                        // number will be needed, but I suppose this allows for the potential to use
                                        // this software on super-computers with hundreds of CPUs.


        double get_timestamp_num() const;  // CHECK: In the interest of full disclosure, I concede do not have a compelling reason
                                    // for choosing a float over a double.
                                    // There may come a time when it becomes clear that a float is not enough.
                                    // An indicator of this would be negative timestamp values.
                                    //
                                    // UPDATE: I've made the decision to use a double.  If we want it to be a float, just
                                    // cast it.  I do not claim this is a perfect or permanent solution.
        
        //uint_fast8_t get_flags() const; // This should return the flags data member, not the flags portion of the raw_data.

    protected:

        virtual void print( ostream& s_out ) const;
};
