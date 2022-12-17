.PHONY: build run clean

TARGET = fork

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SOURCE_DIR = source
BUILD_DIR = build
OBJ_DIR = objects

SOURCES = fork.c
OBJECTS = $(SOURCES:.c=.o)

build: $(TARGET)

run: build
	./$(BUILD_DIR)/$(TARGET)

$(TARGET): fork.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/fork.o -o $(BUILD_DIR)/$@ $(CFLAGS)

fork.o: $(SOURCE_DIR)/fork.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)