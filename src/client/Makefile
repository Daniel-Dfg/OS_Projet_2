# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Boost library
BOOST_LIBS = -lboost_system -lboost_thread
BOOST_INCLUDE = # Add your Boost include path here if needed
BOOST_LIB_PATH = # Add your Boost library path here if needed

# Source files and executable name
SRC = main.cpp client.cpp
EXEC = client_app

# Default target
all: $(EXEC)

# Build the executable
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(BOOST_INCLUDE) $(SRC) -o $(EXEC) $(BOOST_LIB_PATH) $(BOOST_LIBS)

# Clean build files
clean:
	rm -f $(EXEC)

# Phony targets
.PHONY: all clean
