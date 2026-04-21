# Detect OS
ifeq ($(OS),Windows_NT)
    EXE_EXT := .exe
    RM := del /Q
    MKDIR := mkdir
    SEP := \\
else
    EXE_EXT :=
    RM := rm -f
    MKDIR := mkdir -p
    SEP := /
endif

CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference

LIB_DIR := lib

# -------- LIBRARY --------
LIB_NAME := libcstring.a
LIB_SRC := cstring.c
LIB_OBJ := $(LIB_DIR)/cstring.o

# -------- APP --------
SRC := Main.c
OBJ := $(LIB_DIR)/Main.o
TARGET := obj$(EXE_EXT)

# Default
all: compile

# Ensure lib dir exists
$(LIB_DIR):
	@$(MKDIR) $(LIB_DIR)

# Compile library object
$(LIB_DIR)/cstring.o: cstring.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main object
$(LIB_DIR)/Main.o: Main.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build library
$(LIB_NAME): $(LIB_OBJ)
	ar rcs $@ $^

# Build everything
compile: $(LIB_NAME) $(OBJ)
	$(CC) $(OBJ) -L. -lcstring -o $(TARGET)

# Run
run: compile
	./$(TARGET)

# Clean
clean:
	$(RM) $(LIB_DIR)$(SEP)*.o
	$(RM) $(LIB_NAME)
	$(RM) $(TARGET)