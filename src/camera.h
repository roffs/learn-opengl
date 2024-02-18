#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Camera
{
public:
    glm::vec3 position;

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;

    float speed;

    Camera(glm::vec3 position, float yaw, float pitch, float speed)
    {
        this->position = position;
        this->speed = speed;
        this->yaw = yaw;
        this->pitch = pitch;

        forward = glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)), sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));

        up = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::normalize(glm::cross(forward, up));
        up = glm::cross(right, forward);
    };

    glm::mat4x4 getView()
    {
        return glm::lookAt(position, position + forward, up);
    }
};