cmake_minimum_required ( VERSION 3.0 )

if ( UNIX )
    list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/linux_syscall_reader.cpp" )
endif( UNIX )

list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/reader_factory.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
