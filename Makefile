# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -O3 -ffast-math -pedantic-errors -Iinclude -I ${PATH_TO_FFML}/include

# Linker flags
LDFLAGS = -L ${PATH_TO_FFML}/lib

# Libraries to link against
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files directory
SRC_DIR = src

# Object files directory
OBJ_DIR = obj

# Binary directory
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Binary target
BIN = $(BIN_DIR)/main

.PHONY: all clean

all: $(BIN)

debug: CXXFLAGS += -g -pg -fprofile-arcs -ftest-coverage
debug: CXXFLAGS := $(filter-out -O3,$(CXXFLAGS))
debug: LDFLAGS += -lgcov --coverage -pg
debug: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(BIN)