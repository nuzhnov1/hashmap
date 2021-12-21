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
INCLUDE         := ./include
SRC             := ./src

# Varriables for program
PROG_NAME   := hashtable

PROG_SRCS   := $(wildcard $(SRC)/*.cpp)
PROG_OBJS   := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(PROG_SRCS))
PROG_BIN    := $(BIN)/$(PROG_NAME)


# Phony targets
.PHONY: program test debug clean tar


# Default target
all: program


# Build program target
program: $(PROG_BIN)
	$(info Building a program is complete. Executable file is located \
	in "$(BIN)" directory.)

# Test target
test: CFLAGS    := -O3 -std=c++11 -Wall -Wpedantic -DTEST

# Debug target
debug: CFLAGS	:= -g -std=c++11 -Wall -Wpedantic -DTEST
debug: program

# Clean target
clean:
	$(info Removing a directories "$(OBJ)" and "$(BIN)"...)
	$(RMDIR) $(OBJ) $(BIN)

# Create "tar" target
tar:
	$(info Archiving the project...)
	$(TAR) $(PROG_NAME).tar $(INCLUDE) $(SRC) Makefile
	$(info Project has archived. Archive file: $(PROG_NAME).tar)


# Creating directories target
$(BIN) $(OBJ):
	$(info Creating a directory "$@"...)
	$(MKDIR) $@


# Compilation cpp files target
$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	$(info Compiling a "$<" file...)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

# Linkage program target
$(PROG_BIN): $(PROG_OBJS) | $(BIN)
	for item in $^ ; do \
		echo "Linking a $$item file..." ; \
	done
	$(CC) $^ -o $@
