#include <iostream>
#include <core/Window.hpp>
#include <core/Application.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->window->setSize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->camera.updateEuler(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->camera.updateFOV(xoffset, yoffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        app->xRayToggled = !app->xRayToggled; // Toggle the switch
    }
}

Window::Window(int width, int height, std::string title) {
    window = nullptr;
    this->width = width;
    this->height = height;
    this->title = title;
}

bool Window::initialize() {
    // init glfw
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void Window::shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::clear() const {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

int Window::getWidth() { return width; }

int Window::getHeight() { return height; }

GLFWwindow* Window::getGLFWWindow() { return window; }