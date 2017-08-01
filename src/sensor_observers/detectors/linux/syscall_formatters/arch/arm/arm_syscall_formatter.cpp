/*
 *  File Name : arm_syscall_formatter.cpp
 *  
 *  Creation Date : 12-26-2016
 *
 *  Last Modified : Mon 31 Jul 2017 05:18:50 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "arm_syscall_formatter.h"

ARM_Syscall_Formatter::ARM_Syscall_Formatter(){

    //  Nothing needs to be done for the constructor.
}

/* This function takes a raw system call number and returns a value within the range 0-395 in
 * thumb mode.  These numbers are offset by 0x900000 outside of thumb or EABI mode.
 * Currently, the default system calls for ARM range from 0 to 390, but there are 5 proprietary
 * calls that number from 983041 through 983045 (inclusive).  To keep the numbers in a usable
 * range, this function maps the first 391 (0-390) system calls to values in the range 0-390,
 * regardless of whether the OABI, EABI, or Thumb Mode is used.  The 5 proprietary calls
 * are re-mapped to values ranging from 391-395 (inclusive).  Thus, there are 396 system
 * calls mapped to the range 0-395.
 */

uint_fast32_t ARM_Syscall_Formatter::format_syscall_num( uint_fast32_t syscall_num ){

    return ( syscall_num & BIT_MASK ) + ( last_syscall_ ) * ( syscall_num / PRIVATE_OFFSET );
}

/* This function takes the formatted system call and returns the actual value as described in the linux kernel
 * headers for ARM.  Formatted system call numbers in range 0-390 retain their value.  Formatted system call 
 * numbers in range 391-395 will be mapped to their value modulo 390 plus the private offset.
 * These aren't guaranteed to be the original values because we don't know whether these were collected from
 * a system using EABI/Thumb or OABI, so all of these values might be 0x00900000 (the STRONG_OFFSET) less
 * than they were originally.
 */

uint_fast32_t ARM_Syscall_Formatter::recover_syscall_num( uint_fast32_t formatted_syscall_num ){

    return ( formatted_syscall_number % first_private_syscall_ ) + (formatted_syscall_num / first_private_syscall_ ) * ( PRIVATE_OFFSET + 1 )
}

uint_fast32_t ARM_Syscall_Formatter::table_size() const{

    return table_size_;
}
