cmake_minimum_required ( VERSION 3.0 )

add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/command_line_utils/" )
add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/sensor_manager/" )

list ( APPEND SOURCE " ${CMAKE_CURRENT_SOURCE_DIR}/launcher.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
