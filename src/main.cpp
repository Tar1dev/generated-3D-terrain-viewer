#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader_loader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"


int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;
const char *WINDOW_TITLE = "ToonBox";

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    

    //loading shaders
    unsigned int vertexShader, fragmentShader;
    vertexShader = loadShader(GL_VERTEX_SHADER, "vertex.vert");
    if (!vertexShader) {
        std::cerr << "Failled to load the vertex shader" << std::endl;
        glfwTerminate();
    }
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, "fragment.frag");
    if (!fragmentShader) {
        std::cerr << "Failled to load the fragment shader" << std::endl;
        glfwTerminate();
    }
    ShaderProgram shaderProgram(vertexShader, fragmentShader);
    shaderProgram.cleanShaders();

    unsigned int lightingVertexShader, lightingFragmentShader;
    lightingVertexShader = loadShader(GL_VERTEX_SHADER, "lighting_source.vert");
    lightingFragmentShader = loadShader(GL_FRAGMENT_SHADER, "lighting_source.frag");
    ShaderProgram lightingShader(lightingVertexShader, lightingFragmentShader);
    lightingShader.cleanShaders();


    float vertices[] = {
        // position          // texture       // normals
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,     0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0f, 0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,     0.0f, 0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,     0.0f, 0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     -1.0f, 0.0f, 0.0f,
    
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,     0.0f,  -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,     0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,     0.0f, -1.0f,  0.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f, 1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,     0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,     0.0f, 1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,     0.0f, 1.0f,  0.0f,
    };    
    
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices), mesh.indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // normals vectors
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnable(GL_DEPTH_TEST);

    Texture container("assets/container2.png");
    Texture container_specular("assets/container2_specular.png");


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    std::vector<glm::vec3> cubes_positions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.2f, -1.0f, -2.0f),
    };
    

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();
        shaderProgram.setVec3("lightPos",  lightPos);
        shaderProgram.setVec3("viewPos", camera.getPos());

        shaderProgram.setInt("material.diffuse", 0);
        shaderProgram.setInt("material.specular", 1);
        shaderProgram.setFloat("material.shininess",  30.f);
        shaderProgram.setVec3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
        shaderProgram.setVec3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
        shaderProgram.setVec3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
        shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        glm::mat4 model         = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFOV()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        // camera/view transformation
        view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getFront(), camera.getUp());

        // pass transformation matrices to the shader
        shaderProgram.setMatrix("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shaderProgram.setMatrix("view", view);
        
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, container.getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, container_specular.getID());

        for (int i = 0; i < cubes_positions.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubes_positions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.f), glm::vec3(0.0f, 1.0f, 0.0f));
            shaderProgram.setMatrix("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        



        lightingShader.use();
        lightingShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setMatrix("projection", projection);
        lightingShader.setMatrix("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        lightingShader.setMatrix("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


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