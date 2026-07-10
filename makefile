# ============================================
#  utensils Makefile
# ============================================

CC = gcc
AR = ar
CFLAGS = -Wall -O2 -I include -I src

TARGET = libutensils.a

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

SOURCES = $(shell find $(SRC_DIR) -name "*.c")
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))


all: $(LIB_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

print:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"

.PHONY: all clean print