CC = clang
CFLAGS = -O2 -Wall -Wextra -I. -I../../include
LDFLAGS = -L../../lib -lm -lutensils

SRC_DIR = .
BUILD_DIR = build

TARGET = bench_all

SOURCES = $(shell find $(SRC_DIR) -name "*.c")
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) results/*.log

.PHONY: all run clean