#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

#if !defined(SHADER_LOADER_H)
#define SHADER_LOADER_H

char* readShaderSource(std::string file_name) {

    std::ostringstream oss;
    oss << "./assets/shaders/" << file_name;
    
    std::ifstream vertexShaderSourceStream(oss.str(), std::ios::in | std::ios::binary);

    if (!vertexShaderSourceStream.is_open()) {
        std::cerr << "Error opening the shader source!" << std::endl;
    }

    vertexShaderSourceStream.seekg(0, std::ios::end);
    std::streamsize fileSize = vertexShaderSourceStream.tellg();  // Get file size
    vertexShaderSourceStream.seekg(0, std::ios::beg);

    // Allocate memory for the file content
    char* buffer = new char[fileSize + 1];  // +1 for null terminator (if needed)

    // Read the content of the file into the buffer
    if (vertexShaderSourceStream.read(buffer, fileSize)) {
        // Add a null terminator (in case the file is treated as a C-style string)
        buffer[fileSize] = '\0';
    }

    return buffer;
}

unsigned int loadShader(unsigned int shaderType, std::string fileName) {
    const char* shaderSource = readShaderSource(fileName);

    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    return shader;
}

#endif // SHADER_LOADER_H
