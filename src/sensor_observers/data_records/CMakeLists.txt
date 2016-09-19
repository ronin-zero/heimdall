cmake_minimum_required ( VERSION 3.0 )

list ( APPEND SOURCE " ${CMAKE_CURRENT_SOURCE_DIR}/data_record.cpp" )

if ( UNIX )
    add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/linux/" )
endif ( UNIX )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
