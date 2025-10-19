# Compiler and flags
CC = gcc
# CFLAGS are for compilation. -g adds debug symbols for GDB. -I./include tells the compiler to look for headers in the 'include' folder.
CFLAGS = -g -I./include -Wall
# LDFLAGS are for linking.
LDFLAGS = -lglfw -lGLEW -lGL -lm

# List ALL your SOURCE (.c) files here.
# IMPORTANT: Use the correct paths to your .c files.
SRC = src/main.c \
      src/Engine.c \
      src/Camera.c \
      src/Shader.c \
      src/ModelLoader.c \
      src/TextureLoader.c \
      src/Skybox.c

# Automatically generate object (.o) file names from source files
OBJ = $(SRC:.c=.o)

# The name of your final executable
TARGET = bin/main

# The default rule: build the target executable
all: $(TARGET)

# Rule to link the final executable from all the object files
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Rule to compile any .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up all compiled files
clean:
	rm -f $(OBJ) $(TARGET)