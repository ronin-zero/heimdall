/*
 *  File Name : arm_syscall_formatter.h
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Wed 08 Mar 2017 12:27:32 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint> 

#include "sensor_observers/detectors/linux/syscall_formatters/syscall_formatter.h"


class ARM_Syscall_Formatter : public Syscall_Formatter{

    public:

        ARM_Syscall_Formatter();
        ~ARM_Syscall_Formatter() {}

        uint_fast32_t format_syscall_num( uint_fast32_t syscall_num );
        uint_fast32_t recover_syscall_num( uint_fast32_t formatted_syscall_num );

    private:

        static const uint_fast32_t STRONG_OFFSET    = 0x00900000;   /*  In "thumb" mode, ARM syscalls begin at 0.  In "Strong" mode,
                                                                     *  they begin at 9437184 (decimal), 0x0090000 (hexidecimal).
                                                                     */

        static const uint_fast32_t OUTLIER_SYSCALL  = 0x000F0005;   /*  This value rarely appears, but it sometimes does.  It has a 
                                                                     *  decimal value of 983045.  It is (apparently) the syscall
                                                                     *  set_tls.  Why it has this number so far outside of the range
                                                                     *  of the others in either "strong" or "thumb" mode is unclear.
                                                                     */

        static const uint_fast32_t FORMATTED_OUTLIER = 0x00000185;  /*  This is what the OUTLIER_SYSCALL becomes after formatting. */
                                                                    


        static const uint_fast32_t BIT_MASK        = 0x000001FF;    /*  When adjusted, ARM syscalls have a range of 0-377.  The outlier 
                                                                     *  value that sometimes appears is 983045.  When adjusted, it takes 
                                                                     *  on the value 389 (decimal), 0x00000185 (hexidecimal).  0-377 can 
                                                                     *  be expressed with just 9 bits.  Conveniently, 389 can be
                                                                     *  included with the other syscalls within the same datatype and 
                                                                     *  order of magnitude (in decimal, binary, and hexidecimal).  Thus, 
                                                                     *  we need a bitmask of nine 1's, or 0x01FF.
                                                                     */

        static const uint_fast32_t BIT_SHIFT        = 9;            /* See the comment on BIT_MASK for explanation.  This is just to 
                                                                     *  avoid magic numbers by placing 9 in a constant.
                                                                     */


};
