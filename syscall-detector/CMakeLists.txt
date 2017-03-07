cmake_minimum_required ( VERSION 3.0 )

project (Heimdall)

set( targetname "syscall-detector" )

message( "At the start, our CMAKE_CXX_FLAGS are: ${CMAKE_CXX_FLAGS}" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -std=c++11 -pthread" )

message( "At the start, our CMAKE_CXX_FLAGS_DEBUG are: ${CMAKE_CXX_FLAGS_DEBUG}" )

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0 -DDEBUG_COUT" )

set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS}" )

message( "CMAKE_SOURCE_DIR is: ${CMAKE_SOURCE_DIR}" )

if ( TARGET "syscall-detector" )
    message( "Target is ${TARGET}" )
endif( TARGET "syscall-detector" )


set( INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../src/" )
set( SOURCE_DIR "${CMAKE_SOURCE_DIR}/../src/" )
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

set( SENSOR_PROGRAM "${SOURCE_DIR}executables/")

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

message( "Just seeing if this is a thing: ${CXX_FLAGS}" )

set( SOURCE "" )

# These are basically always added.

add_subdirectory( ${SENSOR_DATA} ${BIN_DIR}/sensor_data )
add_subdirectory( ${UTILS} ${BIN_DIR}/utils )
add_subdirectory( ${SENSOR_PROGRAM} ${BIN_DIR}/executables )
add_subdirectory( ${SYSCALL_SENSORS} ${BIN_DIR}/syscall_sensors )
add_subdirectory( ${SENSOR_OBSERVERS} ${BIN_DIR}/sensor_observers )

message( "Libs to Link: ${CMAKE_THREAD_LIBS_INIT}" )

add_executable( syscall-detector "${SOURCE}" )

target_link_libraries( syscall-detector ${CMAKE_THREAD_LIBS_INIT} )

install( TARGETS syscall-detector RUNTIME DESTINATION /usr/sbin/ )
