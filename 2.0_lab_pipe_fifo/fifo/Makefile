.PHONY: build run clean

TARGET = fifo

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SOURCE_DIR = source
BUILD_DIR = build
OBJ_DIR = objects

SOURCES =fifo.c
OBJECTS = $(SOURCES:.c=.o)

build: $(TARGET)

run: build
	./$(BUILD_DIR)/$(TARGET)

$(TARGET): fifo.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/fifo.o -o $(BUILD_DIR)/$@ $(CFLAGS)

fifo.o: $(SOURCE_DIR)/fifo.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)
	rm fifo_file