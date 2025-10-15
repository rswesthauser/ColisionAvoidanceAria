# Makefile for the collision avoidance project

# Compiler and flags
CXX = g++
CXXFLAGS = -I/usr/local/Aria/include -I./include
CXXFLAGS += -Wno-deprecated-declarations
LDFLAGS = -L/usr/local/Aria/lib -lAria

# Directories
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include

# Source and Object files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Final target
TARGET = build/main

# Default target to build
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Rule for compiling .cpp files into .o (object files)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
