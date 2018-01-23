cmake_minimum_required ( VERSION 3.0 )

list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/syscall_formatter.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/arch/arm/arm_syscall_formatter.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/arch/mips/mips_syscall_formatter.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/arch/mips/mips_n32_syscall_formatter.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/arch/mips/mips_n64_syscall_formatter.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/arch/mips/mips_o32_syscall_formatter.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
