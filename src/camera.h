#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    void updateLocalAxis()
    {
        forward = glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)), sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)));

        up = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::normalize(glm::cross(forward, up));
        up = glm::cross(right, forward);
    }

public:
    glm::vec3 position;

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;

    float translationSpeed;
    float rotationSpeed;

    Camera(glm::vec3 position, float yaw, float pitch, float translationSpeed, float rotationSpeed)
    {
        this->position = position;
        this->translationSpeed = translationSpeed;
        this->rotationSpeed = rotationSpeed;
        this->yaw = yaw;
        this->pitch = pitch;

        updateLocalAxis();
    };

    void rotate(float xoffset, float yoffset)
    {
        yaw += xoffset * rotationSpeed;
        pitch += yoffset * rotationSpeed;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateLocalAxis();
    }

    glm::mat4x4 getView()
    {
        return glm::lookAt(position, position + forward, up);
    }
};