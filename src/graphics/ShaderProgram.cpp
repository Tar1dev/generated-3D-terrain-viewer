#include <glad/glad.h>
#include <string>
#include <graphics/ShaderProgram.h>
#include <graphics/shader_loader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ShaderProgram::ShaderProgram(std::string vertexShaderPath, std::string fragmentShaderPath) {
    this->vertexShaderPath = vertexShaderPath;
    this->fragmentShaderPath = fragmentShaderPath;
    this->ID = glCreateProgram();
}

bool ShaderProgram::initialize() {
    this->vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderPath);
    if (!vertexShader) {
        std::cerr << "Failled to load the vertex shader" << std::endl;
        return false;
    }
    this->fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
    if (!fragmentShader) {
        std::cerr << "Failled to load the fragment shader" << std::endl;
        return false;
    }

    glAttachShader(ID, this->vertexShader);
    glAttachShader(ID, this->fragmentShader);
    glLinkProgram(ID);
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, 0, infoLog);
        std::cerr << infoLog << std::endl;
        return false;
    }
    return true;
}


void ShaderProgram::cleanShaders() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use() {
    glUseProgram(this->ID);
}

void ShaderProgram::setBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void ShaderProgram::setInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void ShaderProgram::setFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void ShaderProgram::applyTransform(glm::mat4 trans)
{
    unsigned int transformLoc = glGetUniformLocation(ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void ShaderProgram::setMatrix(std::string name, glm::mat4 matrix)
{
    unsigned int matLoc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setVec3(std::string name, glm::vec3 vector) {
    unsigned int vecLoc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(vecLoc, 1, glm::value_ptr(vector));
}

unsigned int ShaderProgram::getID()
{
    return ID;
}

void ShaderProgram::setMaterial(Material mat) {
    setVec3("material.ambient", mat.ambient);
    setVec3("material.diffuse", mat.diffuse);
    setVec3("material.specular", mat.specular);
    setFloat("material.shininess", mat.shininess * 128.f);
}
