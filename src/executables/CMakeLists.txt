cmake_minimum_required ( VERSION 3.0 )

message( "Target is ${targetname}" )

if ( $targetname STREQUAL "syscall-sensor" )
    add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/sensor_command_line_utils/" )
    add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/sensor_manager/" )
    list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/launcher.cpp" )
elseif( $targetname STREQUAL "syscall-detector" )
    add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/detector_command_line_utils/" )
    add_subdirectory ( "${CMAKE_CURRENT_SOURCE_DIR}/detector_manager/" )
    message ( "Currently in ${CMAKE_CURRENT_SOURCE_DIR}" )
    list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/detector_launcher.cpp" )
endif( $targetname STREQUAL "syscall-sensor" )


set ( SOURCE ${SOURCE} PARENT_SCOPE )
