# Variable definitions
MKDIR   := mkdir -p
RMDIR	:= rm -rf
COPY	:= cp
TAR		:= tar -cf

# Using compilers
CC	:= g++

# Compiler options
CFLAGS := -O3 -std=c++11 -Wall -Wpedantic

# Common directories
BIN             := ./bin
OBJ             := ./obj
LIB				:= ./lib
INCLUDE         := ./include
SRC             := ./src

# Varriables for program
PROG_NAME   := hashmap

PROG_SRCS   := $(wildcard $(SRC)/*.cpp)
PROG_OBJS   := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(PROG_SRCS))
PROG_BIN    := $(BIN)/$(PROG_NAME)

# Varriables for hash library
HASH_LIB_NAME	:= libHash

HASH_SRCS	:= $(wildcard $(SRC)/hash/*.cpp)
HASH_OBJS	:= $(patsubst $(SRC)/hash/%.cpp,$(OBJ)/hash/%.o,$(HASH_SRCS))
HASH_LIB	:= $(LIB)/$(HASH_LIB_NAME).a


# Phony targets
.PHONY: program debug clean tar


# Default target
all: program


# Build program target
program: $(PROG_BIN)
	$(info Building a program is complete. Executable file is located \
	in "$(BIN)" directory.)

# Debug target
debug: CFLAGS	:= -g -std=c++11 -Wall -Wpedantic -DTEST
debug: program

# Clean target
clean:
	$(info Removing a directories "$(OBJ)", "$(LIB)" and "$(BIN)"...)
	$(RMDIR) $(OBJ) $(LIB) $(BIN)

# Create "tar" target
tar:
	$(info Archiving the project...)
	$(TAR) $(PROG_NAME).tar $(INCLUDE) $(SRC) Makefile
	$(info Project has archived. Archive file: $(PROG_NAME).tar)


# Creating directories target
$(BIN) $(LIB) $(OBJ):
	$(info Creating a directory "$@"...)
	$(MKDIR) $@

# Creating directory for library objects target
$(OBJ)/hash: $(OBJ)
	$(info Creating a directory "$@"...)
	$(MKDIR) $@

# Compilation library target
$(OBJ)/hash/%.o: $(SRC)/hash/%.cpp | $(OBJ)/hash
	$(info Compiling a "$<" file...)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Compilation program target
$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	$(info Compiling a "$<" file...)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Create library target
$(HASH_LIB): $(HASH_OBJS) | $(LIB)
	for item in $^ ; do \
		echo "Linking a $$item file..." ; \
	done
	ar rvs $(HASH_LIB) $^

# Linkage program target
$(PROG_BIN): $(PROG_OBJS) $(HASH_LIB) | $(BIN)
	for item in $^ ; do \
		echo "Linking a $$item file..." ; \
	done
	$(CC) $(HASH_LIB) $^ -o $@
