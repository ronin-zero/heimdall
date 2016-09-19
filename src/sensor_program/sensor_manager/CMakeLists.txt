cmake_minimum_required ( VERSION 3.0 )

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/daemon_utils/" )

list ( APPEND SOURCE " ${CMAKE_CURRENT_SOURCE_DIR}/sensor_manager.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
