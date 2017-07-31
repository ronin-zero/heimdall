/*
 *  File Name : arm_syscall_formatter.cpp
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Mon 31 Jul 2017 12:25:17 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "arm_syscall_formatter.h"

ARM_Syscall_Formatter::ARM_Syscall_Formatter(){

    //  Nothing needs to be done for the constructor.
}

// TODO: I'm sure there's a better way to do this, in terms of efficiency.
// I want it to return value between 0 and 395, inclusive.  The if statement
// here seems like it will waste CPU time...

uint_fast32_t ARM_Syscall_Formatter::format_syscall_num( uint_fast32_t syscall_num ){

    uint_fast32_t formatted_syscall_num = ( syscall_num & BIT_MASK );

    if ( syscall_num & PRIVATE_OFFSET )
    {
        formatted_syscall_num += last_syscall;
    }

    return formatted_syscall_num;
}

uint_fast32_t ARM_Syscall_Formatter::recover_syscall_num( uint_fast32_t formatted_syscall_num ){

    /* If the systen call comes from "strong" mode, the offset must be added to the formatted number.
     *
     * If the original call comes from "thumb" mode, the number should be fine to return as-is UNLESS
     * it is the outlier for set_tls (0x000F0005, 983045).  The formatted value of this system call is
     * 389, which is 0x185 in hex.  For now, I'm just going to do this using a basic "if" check.
     * I don't like it this way, but I need to move on.
     *
     * CHECK: Full disclosure here: I'm not sure if this kind of #ifdef block is good practice, here.
     * I'm also not sure if the outlier syscall for set_tls (0x000F0005, 983045) occurs outside of thumb mode,
     * and if so, how.  As such, I've included this soft task:
     *
     * TODO: consult someone who knows more than I do about the safety/reliability/best practice for this sort
     * of thing.
     */

    uint_fast32_t syscall_num = formatted_syscall_num;

#ifdef __thumb__
    if ( formatted_syscall_num == FORMATTED_OUTLIER )
    {
        syscall_num = OUTLIER_SYSCALL;
    }
#else
    syscall_num |= STRONG_OFFSET;
#endif

    return syscall_num;

}

uint_fast32_t ARM_Syscall_Formatter::table_size() const{

    return table_size_;
}
