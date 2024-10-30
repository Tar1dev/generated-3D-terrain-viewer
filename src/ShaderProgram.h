#if !defined(SHADER_PROGRAM_H)
#define SHADER_PROGRAM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Material.h"

class ShaderProgram
{
private:
    unsigned int ID;
    unsigned int vertexShader;
    unsigned int fragmentShader;
public:
    ShaderProgram() = default;
    ShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
    void cleanShaders();
    void use();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void applyTransform(glm::mat4 trans);
    void setMatrix(std::string name, glm::mat4 matrix);
    void setVec3(std::string name, glm::vec3 vector);
    unsigned int getID();
    void setMaterial(Material mat);
};


#endif // SHADER_PROGRAM_H
