# =============================================================================
# 1. Configuration - Adjust these variables
# =============================================================================

# --- Compilers ---
CXX = g++
CC = gcc

# --- Executable Name ---
EXECUTABLE = bin/solar_system

# --- Directories ---
SRC_DIR = src
OBJ_DIR = bin/obj
IMGUI_DIR = include/imgui

# --- Compiler Flags ---
# -g: Adds debugging info
# -Wall -Wextra: Enables most warnings
CXXFLAGS = -std=c++17 -g -Wall -Wextra
CFLAGS   = -g -Wall -Wextra

# --- Include Directories ---
# -I tells the compiler where to look for header files (.h)
INC_DIRS = -Iinclude -I$(IMGUI_DIR)

# --- Linker Flags & Libraries ---
# UPDATED for Linux with GLEW instead of GLAD
LDFLAGS =
LIBS    = -lglfw -lGLEW -lGL -lm

# =============================================================================
# 2. Source File Discovery - This section automatically finds files
# =============================================================================

# --- Your Project Sources ---
# Finds all .c and .cpp files in your src directory
SOURCES_C   = $(wildcard $(SRC_DIR)/*.c)
SOURCES_CPP = $(wildcard $(SRC_DIR)/*.cpp)

# --- ImGui Library Sources ---
# We explicitly list the necessary ImGui files to compile
# This assumes you are using the GLFW and OpenGL3 backend
IMGUI_SOURCES = $(addprefix $(IMGUI_DIR)/, \
    imgui.cpp \
    imgui_draw.cpp \
    imgui_tables.cpp \
    imgui_widgets.cpp \
    backends/imgui_impl_glfw.cpp \
    backends/imgui_impl_opengl3.cpp)

# --- Object File Generation ---
# Creates a list of object files (.o) to be placed in the OBJ_DIR
OBJECTS_C   = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES_C))
OBJECTS_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES_CPP))
OBJECTS_IMGUI = $(patsubst $(IMGUI_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(IMGUI_SOURCES))

# --- All Objects ---
OBJECTS = $(OBJECTS_C) $(OBJECTS_CPP) $(OBJECTS_IMGUI)

# --- VPATH ---
# Tells 'make' where to find source files
VPATH = $(SRC_DIR) $(IMGUI_DIR) $(IMGUI_DIR)/backends

# =============================================================================
# 3. Build Rules - These are the instructions for building the project
# =============================================================================

# Default target: build the executable
all: $(EXECUTABLE)

# Rule to link the final executable
$(EXECUTABLE): $(OBJECTS)
	@echo "🔗 Linking executable..."
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "✅ Build finished: $(EXECUTABLE)"

# Rule to compile .cpp and .c files into object files in the OBJ_DIR
$(OBJ_DIR)/%.o: %.cpp
	@echo "Compiling C++: $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@echo "Compiling C:   $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# Rule to clean up build files
clean:
	@echo "🔥 Cleaning up build files..."
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

# Phony targets are not files
.PHONY: all clean