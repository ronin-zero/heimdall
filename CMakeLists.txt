cmake_minimum_required ( VERSION 3.0 )

project (Heimdall)

set( targetname "syscall-sensor" )



set( CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -std=c++11 -pthread" )

set( CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0" )

set( INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/" )
set( SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/" )
set( SENSORS "${SOURCE_DIR}sensors/" )
set( SYSCALL_SENSORS "${SENSORS}syscall_sensors/" )
set( SENSOR_OBSERVERS "${SOURCE_DIR}sensor_observers" )
set( SENSOR_DATA "${SOURCE_DIR}sensor_data/" )
set( UTILS "${SOURCE_DIR}utils/" )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    message( "===DEBUG MODE===" )
    #set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-unused-variable" )
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    message( "===RELEASE MODE===" )
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )

set( BIN_DIR "${CMAKE_SOURCE_DIR}/bin" )
set( DEBUG_DIR "${CMAKE_SOURCE_DIR}/debug" )

set( SENSOR_PROGRAM "${SOURCE_DIR}sensor_program/")

find_package( Threads )

include_directories( ${INCLUDE_DIR} )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DEBUG_DIR} ) 

message( "CMAKE_RUNTIME_OUTPUT_DIRECTORY is: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
message( "targetname is ${targetname} " )
message( "CXX Flags: ${CMAKE_CXX_FLAGS}" )
message( "CXX Debug Flags: ${CMAKE_CXX_FLAGS_DEBUG}" )
message( "CXX Release Flags: ${CMAKE_CXX_FLAGS_RELEASE}" )
message( "CMake Shared Linker Flags: ${CMAKE_SHARED_LINKER_FLAGS}" )

set( SOURCE "" )

# These are basically always added.

add_subdirectory( ${SENSOR_DATA} )
add_subdirectory( ${UTILS} )
add_subdirectory( ${SENSOR_PROGRAM} )
add_subdirectory( ${SYSCALL_SENSORS} )
add_subdirectory( ${SENSOR_OBSERVERS} )

message( "Libs to Link: ${CMAKE_THREAD_LIBS_INIT}" )

add_executable( syscall-sensor "${SOURCE}" )

target_link_libraries( syscall-sensor ${CMAKE_THREAD_LIBS_INIT} )

install( TARGETS syscall-sensor RUNTIME DESTINATION /usr/sbin/ )
