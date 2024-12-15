# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -pedantic -O2

# Target executable
TARGET = server

# Source files
SRCS = server.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilation rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# PHONY targets
.PHONY: all clean
