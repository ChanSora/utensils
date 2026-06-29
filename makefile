# Makefile - 专业版（已修复）
CC = clang
CFLAGS = -Wall -O2 -I./include
LDFLAGS = -lm
AR = ar
ARFLAGS = rcs

# 目录
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
TEST_DIR = tests
EXAMPLE_DIR = examples
BUILD_DIR = build

# 库的目标文件
LIB_SRCS = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIB_SRCS))
LIB_NAME = mathlib.a
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)

# $(info LIB_SRCS = $(LIB_SRCS))   # 加在 LIB_SRCS 定义后面
# $(info LIB_OBJS = $(LIB_OBJS))   # 加在 LIB_OBJS 定义后面

# 测试和示例
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
EXAMPLE_SRCS = $(wildcard $(EXAMPLE_DIR)/*.c)

# 默认目标：编译库 + 示例程序
all: $(LIB_TARGET) examples

# 编译 .o 文件（这个规则是完整的，保留不动）
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译库（修复后的规则）
$(LIB_TARGET): $(LIB_OBJS)
	mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^


# 编译示例程序
examples: $(LIB_TARGET)
	for ex in $(EXAMPLE_SRCS); do \
		name=$$(basename $$ex .c); \
		$(CC) $(CFLAGS) $$ex $(LIB_TARGET) $(LDFLAGS) -o $(EXAMPLE_DIR)/$$name; \
	done

# 编译测试程序（如果写了的话）
tests: $(LIB_TARGET)
	for test in $(TEST_SRCS); do \
		name=$$(basename $$test .c); \
		$(CC) $(CFLAGS) $$test $(LIB_TARGET) $(LDFLAGS) -o $(TEST_DIR)/$$name; \
	done

# 清理
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(EXAMPLE_DIR)/*_demo $(TEST_DIR)/*_test

.PHONY: all examples tests clean