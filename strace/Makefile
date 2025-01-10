# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic

# Target Program
TARGET = strace_0

# Source Files
SRC = strace_0.cpp

# Build Target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean Target
clean:
	rm -f $(TARGET)

# Phony Targets
.PHONY: clean


