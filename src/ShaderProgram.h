#if !defined(SHADER_PROGRAM_H)
#define SHADER_PROGRAM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    unsigned int getID();
};


#endif // SHADER_PROGRAM_H