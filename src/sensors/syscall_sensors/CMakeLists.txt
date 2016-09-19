cmake_minimum_required ( VERSION 3.0 )

add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/syscall_readers/" )

list ( APPEND SOURCE " ${CMAKE_CURRENT_SOURCE_DIR}/syscall_sensor.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
