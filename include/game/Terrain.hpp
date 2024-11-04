#if !defined(TERRAIN_H)
#define TERRAIN_H

#include <glad/glad.h>
#include <vector>

class Terrain
{
private:
    unsigned int VBO, EBO, VAO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
public:
    Terrain();
    void initialize();
    void draw();
};



#endif // TERRAIN_H
