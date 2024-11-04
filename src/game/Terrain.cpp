#include <game/Terrain.hpp>

Terrain::Terrain(siv::PerlinNoise::seed_type seed) {
    this->seed = seed;
}

void Terrain::initialize() {
	const siv::PerlinNoise perlin{ seed };

    const int terrain_witdh = 100;
    const int terrain_height = 100;
    std::vector<std::vector<float>> heightMap(terrain_witdh, std::vector<float>(terrain_height));
	
	for (int x = 0; x < terrain_height; ++x)
	{
		for (int y = 0; y < terrain_witdh; ++y)
		{
			float nx = (float)x / terrain_witdh - 0.5f, ny = (float)y / terrain_height - 0.5f;
            heightMap[x][y] = perlin.octave2D_01((nx), (ny), 4); // Scale up to make the terrain more interesting
		}
	}

    
    int maxHeight = 50;

    for (int x = 0; x < terrain_witdh; x++) {
        for (int y = 0; y < terrain_height; y++) {
            // Position of vertex based on heightmap
            float posX = (float)x;
            float posY = heightMap[x][y] * maxHeight - maxHeight; // maxHeight is a scaling factor for the height
            float posZ = (float)y;

            vertices.push_back(posX);
            vertices.push_back(posY);
            vertices.push_back(posZ);
        }
    }
    

    // Generating indices for a grid of triangles
    for (int x = 0; x < terrain_witdh - 1; x++) {
        for (int y = 0; y < terrain_height - 1; y++) {
            int topLeft = x * terrain_height + y;
            int topRight = (x + 1) * terrain_height + y;
            int bottomLeft = x * terrain_height + (y + 1);
            int bottomRight = (x + 1) * terrain_height + (y + 1);

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Terrain::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}