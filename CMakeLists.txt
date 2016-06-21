cmake_minimum_required ( VERSION 3.5.1 )

project (Heimdall)

set( CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -std=c++11 -pthread" )

#set( CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0" )

set( INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/" )
set( SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/" )
set( SENSORS "${SOURCE_DIR}sensors/" )
set( SYSCALL_SENSORS "${SENSORS}syscall_sensors/" )
set( SENSOR_OBSERVERS "${SOURCE_DIR}sensor_observers" )
set( SENSOR_DATA "${SOURCE_DIR}sensor_data/" )
set( UTILS "${SOURCE_DIR}utils/" )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    message( "===DEBUG MODE===" )
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    message( "===RELEASE MODE===" )
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )

message( "CXX Flags: ${CMAKE_CXX_FLAGS}" )
message( "CXX Debug Flags: ${CMAKE_CXX_FLAGS_DEBUG}" )
message( "CXX Release Flags: ${CMAKE_CXX_FLAGS_RELEASE}" )
message( "CMake Shared Linker Flags: ${CMAKE_SHARED_LINKER_FLAGS}" )

set( BIN_DIR "${CMAKE_SOURCE_DIR}/bin/" )

set( TESTS "${SOURCE_DIR}tests/")

find_package( Threads )

include_directories( ${INCLUDE_DIR} )

set( SOURCE "" )

# These are basically always added.

add_subdirectory( ${SENSOR_DATA} )
add_subdirectory( ${UTILS} )

add_subdirectory( ${TESTS} ${BIN_DIR} )
add_subdirectory( ${SYSCALL_SENSORS} )
add_subdirectory( ${SENSOR_OBSERVERS} )

add_executable( basic_test "${SOURCE}" )
#add_executable( basic_test src/tests/basic_test.cpp
#    src/sensors/syscall_sensors/syscall_readers/reader_factory.cpp
#    src/sensors/syscall_sensors/syscall_readers/linux_syscall_reader.cpp 
#    src/sensor_data/sensor_data.cpp 
#    src/sensors/syscall_sensors/syscall_sensor.cpp
#    src/sensor_observers/syscall_logger.cpp
#    src/sensor_observers/data_streams/output_stream.cpp )

message( "Libs to Link: ${CMAKE_THREAD_LIBS_INIT}" )

target_link_libraries( basic_test ${CMAKE_THREAD_LIBS_INIT} )
