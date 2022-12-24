.PHONY: build clean

TARGET = main

CC = gcc
CFLAGS = -Wall -Wextra -Werror -lpthread

SOURCE_DIR = source
BUILD_DIR = build
OBJ_DIR = obj

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

build: prog

prog: main.o
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	$(CC) $(OBJ_DIR)/main.o -o $(BUILD_DIR)/$@ $(CFLAGS)

main.o: $(SOURCE_DIR)/main.c
	[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $(OBJ_DIR)/$@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OBJ_DIR)