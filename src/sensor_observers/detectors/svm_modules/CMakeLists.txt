cmake_minimum_required ( VERSION 3.0 )

list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/svm_module.cpp" )
list ( APPEND SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/one_class_svm.cpp" )

set ( SOURCE ${SOURCE} PARENT_SCOPE )
