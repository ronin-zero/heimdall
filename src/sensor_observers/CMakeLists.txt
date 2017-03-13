cmake_minimum_required ( VERSION 3.0 )

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/data_records/" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/data_streams/" )

list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/syscall_logger.cpp" )

if ( ${targetname} STREQUAL "syscall-detector" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/detectors/" )
endif( ${targetname} STREQUAL "syscall-detector" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
