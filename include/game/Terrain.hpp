#if !defined(TERRAIN_H)
#define TERRAIN_H

#include <glad/glad.h>
#include <vector>

#include <utils/PerlinNoise.hpp>


class Terrain
{
private:
    siv::PerlinNoise::seed_type seed;
    unsigned int VBO, EBO, VAO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
public:
    Terrain() = default;
    Terrain(siv::PerlinNoise::seed_type seed);
    void initialize();
    void draw();
};



#endif // TERRAIN_H
