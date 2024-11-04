#if !defined(WINDOW_H)
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


class Window
{
private:
    GLFWwindow* window;
    int width, height;
    std::string title;
public:
    Window(int width, int height, std::string title);
    bool initialize();
    void shutdown();
    void pollEvents() const;
    void clear() const;
    void swapBuffers() const;
    bool shouldClose() const;
    void setSize(int width, int height);
    int getWidth();
    int getHeight();
    GLFWwindow* getGLFWWindow();
};

#endif // WINDOW_H
