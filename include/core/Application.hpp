#if !defined(APPLICATION_H)
#define APPLICATION_H

#include <core/Window.hpp>
#include <game/Camera.h>
#include <graphics/ShaderProgram.h>
#include <game/Terrain.hpp>


class Application
{
private:
    ShaderProgram shader;
    float deltaTime;
    float lastFrame;
    Terrain terrain;
public:
    Application(/* args */);
    Window* window;
    Camera camera;
    bool xRayToggled;
    bool initialize();
    void run();
    void shutdown();
};

#endif // APPLICATION_H
