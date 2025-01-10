CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++11

all: strace_0 strace_1 strace_2

strace_0: strace_0.cpp
	$(CXX) $(CXXFLAGS) -o strace_0 strace_0.cpp

strace_1: strace_1.cpp
	$(CXX) $(CXXFLAGS) -o strace_1 strace_1.cpp

strace_2: strace_2.cpp
	$(CXX) $(CXXFLAGS) -o strace_2 strace_2.cpp

clean:
	rm -f strace_0 strace_1 strace_2

.PHONY: all clean

