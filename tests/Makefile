CC = gcc
CFLAGS = -Wall -Wextra -I..
UNITY_DIR = unity
SRC_DIR = ..
TEST_DIRS = member-functions size-capacity input-output element-access iterators modifiers operations

# Source files
CSTRING_SRC = $(SRC_DIR)/cstring.c
UNITY_SRC = $(UNITY_DIR)/unity.c
MAIN_RUNNER = main_test_runner.c

TEST_SRCS = $(foreach dir,$(TEST_DIRS),$(wildcard $(dir)/*.c))

ALL_SRCS = $(CSTRING_SRC) $(UNITY_SRC) $(MAIN_RUNNER) $(TEST_SRCS)

TARGET = test_suite

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(ALL_SRCS)
	$(CC) $(CFLAGS) $(ALL_SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
