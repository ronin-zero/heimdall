cmake_minimum_required ( VERSION 3.5.1 )

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/data_records/" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/data_streams/" )

list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/syscall_logger.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
