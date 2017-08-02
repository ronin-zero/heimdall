/*
 *  File Name : mips_n64_syscall_formatter.h
 *  
 *  Creation Date : 08-01-2017
 *
 *  Last Modified : Tue 01 Aug 2017 08:17:11 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>

#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_syscall_formatter.h"

class MIPS_n64_Syscall_Formatter : public MIPS_Syscall_Formatter {

    public:
        MIPS_n64_Syscall_Formatter();
};
