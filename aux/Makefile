CXX=g++
CXXFLAGS=-std=c++11 -I ../src/ -Werror -Wall -Wextra -pedantic -pthread

.PHONY: all clean default

default: main

main:	launcher.cpp command_line_parser.o ascii_operations.o
	$(CXX) $(CXXFLAGS) launcher.cpp command_line_parser.o ascii_operations.o -o $@

clean:
	rm -f main *.o

command_line_parser.o:	command_line_parser.cpp command_line_parser.h ascii_operations.o
	$(CXX) $(CXXFLAGS) -c command_line_parser.cpp

ascii_operations.o:	ascii_operations.cpp ascii_operations.h
	$(CXX) $(CXXFLAGS) -c ascii_operations.cpp
