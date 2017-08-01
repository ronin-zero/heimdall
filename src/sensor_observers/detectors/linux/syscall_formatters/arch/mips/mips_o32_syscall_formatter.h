/*
 *  File Name : mips_o32_syscall_formatter.h
 *  
 *  Creation Date : 07-31-2017
 *
 *  Last Modified : Tue 01 Aug 2017 01:53:19 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_syscall_formatter.h"

class MIPS_o32_Syscall_Formatter : public MIPS_Syscall_Formatter {

    public:
        MIPS_o32_Syscall_Formatter();
}
