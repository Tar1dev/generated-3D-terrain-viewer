#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <vector>

#include <core/Application.hpp>
#include <utils/PerlinNoise.hpp>


Application::Application() {
    window = nullptr;
    xRayToggled = true;
}

bool Application::initialize() {
    window = new Window(1920, 1080, "3D Terrain generation");
    if (!window->initialize()) {
        std::cerr << "Failed to initialize the window." << std::endl;
        return false;
    }

    shader = ShaderProgram("terrain.vert", "terrain.frag");
    if (!shader.initialize()) {
        std::cerr << "Failed to load shader" << std::endl;
        shutdown();
        return false;
    }
    shader.cleanShaders();
    shader.use();
    
    camera = Camera(window->getWidth(), window->getHeight());
    terrain = Terrain();
    terrain.initialize();


    glfwSetWindowUserPointer(window->getGLFWWindow(), this);


    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);

    deltaTime = 0.0f;
    lastFrame = 0.0f;

    return true;
}

void Application::run() {
    while (!window->shouldClose()) {
        window->clear();

        if (xRayToggled)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader.use();
        
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        glm::mat4 model         = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFOV()), (float)window->getWidth() / (float)window->getHeight(), 0.1f, 100.0f);
        // camera/view transformation
        view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getFront(), camera.getUp());

        // pass transformation matrices to the shader
        shader.setMatrix("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.setMatrix("view", view);
        shader.setMatrix("model", model);

        terrain.draw();
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.update(window->getGLFWWindow(), deltaTime);

        window->swapBuffers();
        window->pollEvents();
    }
}

void Application::shutdown() { window->shutdown(); }
