
CC = gcc

CFLAGS = -Wall -Wextra -Iinclude -g

LDLIBS = -lglfw -lGLEW -lGL -lm -ldl -lpthread

SRC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/main

SRCS_ROOT = $(wildcard *.c)
SRCS_MOD = $(wildcard $(SRC_DIR)/*.c)

OBJS_ROOT = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS_ROOT))
OBJS_MOD = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_MOD))

OBJS = $(OBJS_ROOT) $(OBJS_MOD)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	# The link step. Note we use CC, CFLAGS, $^ (all prerequisites), $@ (the target), and LDLIBS
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	# -c means "compile only, don't link"
	# $< is the first prerequisite (the .c file)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run