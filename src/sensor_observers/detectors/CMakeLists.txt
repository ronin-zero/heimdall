cmake_minimum_required ( VERSION 3.0 )

add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/linux/syscall_formatters/" )
add_subdirectory( "${CMAKE_CURRENT_SOURCE_DIR}/svm_modules/" )

list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/trace_window.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/ngram_generator.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/trace_reader.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/support_vector_generator.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/syscall_detector.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
