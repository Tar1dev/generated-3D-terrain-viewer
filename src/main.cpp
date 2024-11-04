#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader_loader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
// #include "utils/FastNoiseLite.h"
#include "utils/PerlinNoise.hpp"


int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;
const char *WINDOW_TITLE = "Procedural 3D terrain generation";

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
bool xRayToggled = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void) {
    // init glfw
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    

    //loading shaders
    unsigned int vertexShader, fragmentShader;
    vertexShader = loadShader(GL_VERTEX_SHADER, "terrain.vert");
    if (!vertexShader) {
        std::cerr << "Failled to load the vertex shader" << std::endl;
        glfwTerminate();
    }
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, "terrain.frag");
    if (!fragmentShader) {
        std::cerr << "Failled to load the fragment shader" << std::endl;
        glfwTerminate();
    }
    ShaderProgram shader(vertexShader, fragmentShader);
    shader.cleanShaders();

    const siv::PerlinNoise::seed_type seed = 98462626u;

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

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
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


    
    unsigned int VAO, VBO, EBO;
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

    // // texture
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    // glEnableVertexAttribArray(1);

    // // normals vectors
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));
    // glEnableVertexAttribArray(2);

    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader.use();
        // shader.setVec3("viewPos", camera.getPos());

        // shader.setInt("material.diffuse", 0);
        // shader.setInt("material.specular", 1);
        // shader.setFloat("material.shininess",  30.f);

        // // directional light
        // shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        // shader.setVec3("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        // shader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        // shader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));


        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        glm::mat4 model         = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        // camera/view transformation
        view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getFront(), camera.getUp());

        // pass transformation matrices to the shader
        shader.setMatrix("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.setMatrix("view", view);
        shader.setMatrix("model", model);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.update(window, deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{   
    camera.updateEuler(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.updateFOV(xoffset, yoffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        xRayToggled = !xRayToggled; // Toggle the switch
        std::cout << "Toggled: " << xRayToggled << std::endl;
    }
}
