#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
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

    void move(glm::vec3 direction, float deltaTime)
    {
        glm::vec3 m_forward = glm::normalize(glm::vec3(forward.x, 0.0, forward.z));
        glm::vec3 m_up = glm::vec3(0.0, 1.0, 0.0);
        glm::vec3 m_right = glm::cross(m_forward, m_up);

        position += m_forward * direction.x * translationSpeed * deltaTime;
        position += m_right * direction.y * translationSpeed * deltaTime;
        position += m_up * direction.z * translationSpeed * deltaTime;
    }

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

    glm::vec3 getPosition()
    {
        return position;
    }

    glm::vec3 getForward()
    {
        return forward;
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