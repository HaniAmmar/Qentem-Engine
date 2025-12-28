# Choose your compiler
CXX         := c++
CXXFLAGS    := -std=c++17 -O0 -Wall -fno-exceptions -flto=auto -fno-rtti -fno-threadsafe-statics -nostdlib++ -IInclude
BUILD_DIR   := Build
TEST_SRC    := Tests/Test.cpp
TEST_BIN    := $(BUILD_DIR)/QTest.bin

.PHONY: all test clean

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

test: all
	@$(TEST_BIN)

clean:
	rm -f $(TEST_BIN)
