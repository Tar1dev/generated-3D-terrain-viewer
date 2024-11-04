# Variables
CXX = g++
INCLUDES = -I include
LIBS = -L ./libs/GLFW -lglfw3 -lgdi32 -lopengl32 -luser32
SRC_DIR = src
LIB_DIR = libs/glad
BIN_DIR = build/debug
TARGET = $(BIN_DIR)/sandbox.exe

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
LIB_FILES = $(LIB_DIR)/glad.c

# All rule
all: compile run

# Compile rule
compile:
	$(CXX) main.cpp $(SRC_FILES) $(LIB_FILES) -o $(TARGET) $(INCLUDES) $(LIBS)

# Run rule
run:
	.\$(TARGET)

# Clean rule
clean:
	rm -rf $(BIN_DIR)/*.exe
