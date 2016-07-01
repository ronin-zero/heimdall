CXX=g++
CXXFLAGS=-std=c++11

.PHONY: all clean default

default: main

main:	main.cpp command_line_parser.o
	$(CXX) $(CXXFLAGS) main.cpp command_line_parser.o -o $@

clean:
	rm -f main *.o

command_line_parser.o:	command_line_parser.cpp command_line_parser.h ascii_operations.o
	$(CXX) $(CXXFLAGS) -c command_line_parser.cpp

ascii_operations.o:	ascii_operations.cpp ascii_operations.h
	$(CXX) $(CXXFLAGS) -c ascii_operations.cpp
