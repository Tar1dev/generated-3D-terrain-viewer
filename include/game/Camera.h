#if !defined(CAMERA_H)
#define CAMERA_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::vec3 pos;
    glm::vec3 target;
    glm::vec3 direction;
    glm::vec3 _up;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 front;
    glm::mat4 view;
    float lastX, lastY;
    float sensitivity;
    double pitch, yaw;
    bool firstMouse;
    float fov;
public:
    Camera(int width, int height);
    Camera() = default;
    glm::vec3 getPos();
    glm::vec3 getTarget();
    glm::vec3 getDirection();
    glm::vec3 getRight();
    glm::vec3 getUp();
    glm::vec3 getFront();
    glm::mat4 getView();

    void setFront(glm::vec3 new_front);
    void setPos(glm::vec3 new_pos);
    void update(GLFWwindow* window, float dt);
    void updateEuler(double xpos, double ypos);
    void updateFOV(double xoffset, double yoffset);
    float getFOV();
};

#endif // CAMERA_H