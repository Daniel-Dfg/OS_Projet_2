# Compiler
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2 -g

# Target executable
TARGET = chat

# Source files
SRCS = $(wildcard *.cpp)

# Default target
all: clean $(TARGET)

# Compile and link directly to the executable
$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

# Clean up the executable
clean:
	rm -f $(TARGET)
