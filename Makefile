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

# Ensure lib directory exists
LIB_DIR := lib

# Detect src folder
HAS_SRC := $(wildcard src)

# -------- LIBRARY --------
LIB_NAME := libcstring.a
LIB_SRC := cstring.c
LIB_OBJ := $(LIB_DIR)/cstring.o

# -------- APP --------
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,$(LIB_DIR)/%.o,$(SRC))

TARGET := app$(EXE_EXT)

# Default
all: compile

# Ensure lib dir exists
$(LIB_DIR):
	@$(MKDIR) $(LIB_DIR)

# -------- COMPILE LIB OBJECT --------
$(LIB_DIR)/%.o: %.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# -------- COMPILE SRC OBJECTS --------
$(LIB_DIR)/%.o: src/%.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# -------- BUILD LIBRARY --------
$(LIB_NAME): $(LIB_OBJ)
	ar rcs $@ $^

# -------- BUILD LOGIC --------
compile: $(LIB_NAME)
ifdef HAS_SRC
	$(MAKE) build_app
endif

# -------- BUILD APP --------
build_app: $(OBJ)
	$(CC) $(OBJ) -L. -lcstring -o $(TARGET)

# -------- RUN --------
run: compile
ifdef HAS_SRC
	./$(TARGET)
else
	@echo "No executable (no src folder)"
endif

# -------- CLEAN --------
clean:
	$(RM) $(LIB_DIR)$(SEP)*.o
	$(RM) $(LIB_NAME)
ifdef HAS_SRC
	$(RM) $(TARGET)
endif