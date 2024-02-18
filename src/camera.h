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
    float fov;
    float aspect;

    float translationSpeed;
    float rotationSpeed;

    Camera(glm::vec3 position, float yaw, float pitch, float fov, float aspect, float translationSpeed, float rotationSpeed)
    {
        this->position = position;
        this->translationSpeed = translationSpeed;
        this->rotationSpeed = rotationSpeed;
        this->yaw = yaw;
        this->pitch = pitch;
        this->fov = fov;
        this->aspect = aspect;

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

    void zoom(float offset)
    {
        fov -= (float)offset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    glm::mat4x4 getView()
    {
        return glm::lookAt(position, position + forward, up);
    }

    glm::mat4x4 getProjection()
    {
        return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
    }
};