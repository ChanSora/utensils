CC = clang
CFLAGS = -O2 -Wall -Wextra -I. -I./avl_tree -I./rb_tree
LDFLAGS = -lm

# 公共依赖（树实现 + 测试用例）
TREE_SRCS = ./avl_tree/avl_tree.c ./rb_tree/rb_tree.c
BENCH_SRCS = bst_benchmarks.c bench_sequential.c bench_random.c bench_common.c bench_concurrent_find.c
OBJS = $(TREE_SRCS:.c=.o) $(BENCH_SRCS:.c=.o)
TARGET = bench_all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) results/*.log

.PHONY: all run clean