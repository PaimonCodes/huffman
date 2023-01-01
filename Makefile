CC := g++
DEV := -std=c++14

INCLUDE_DIR := include
BUILD_DIR := build
BIN_DIR := bin
DATA_DIR := data
TEST_DIR := tests

TESTS = $(shell find $(TEST_DIR) -name *.cxx)
TEST_EXE = $(patsubst $(TEST_DIR)/%.cxx, $(BUILD_DIR)/%.exe, $(TESTS))

test: $(TEST_EXE)

%.o: %.cpp
	$(CC) $(DEV) -c $< -o $@

$(BUILD_DIR)/%.exe: $(TEST_DIR)/%.cxx include/huffman.o
	$(CC) $(DEV) $^ -o $@

clean:
	rm -f $(TEST_EXE) *.exe
	rm -f *.o
