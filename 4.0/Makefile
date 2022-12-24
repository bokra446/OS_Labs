.PHONY: build clean

TARGET = 1 2

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SOURCE_DIR = source
BUILD_DIR = build
OBJ_DIR = objects

SOURCES = $(wildcard *.c)
OBJS := $(SOURCES:.c=.o)

build: $(TARGET)

$(TARGET): 1.o 2.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/1 $(CFLAGS) $(OBJ_DIR)/1.o
	$(CC) -o $(BUILD_DIR)/2 $(CFLAGS) $(OBJ_DIR)/2.o

1.o:
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $(SOURCE_DIR)/1.c -o $(OBJ_DIR)/1.o $(CFLAGS)

2.o:
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $(SOURCE_DIR)/2.c -o $(OBJ_DIR)/2.o $(CFLAGS)


clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)