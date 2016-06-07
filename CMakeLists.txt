cmake_minimum_required (VERSION 3.5.1)

project (Heimdall)

# The version number.
set (Heimdall_VERSION_MAJOR 1)
set (Heimdall_VERSION_MINOR 0)

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror -pedantic" )

include_directories( ./include )

add_subdirectory( ./src ./build )
