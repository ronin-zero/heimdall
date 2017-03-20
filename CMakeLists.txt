cmake_minimum_required ( VERSION 3.0 )

project (Heimdall)

set( SYSCALL_SENSOR_DIR "${CMAKE_SOURCE_DIR}/syscall-sensor" )
set( SYSCALL_DETECTOR_DIR "${CMAKE_SOURCE_DIR}/syscall-detector" )

add_subdirectory( ${SYSCALL_SENSOR_DIR} )
add_subdirectory( ${SYSCALL_DETECTOR_DIR} )
