#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    float speed;

    Camera(glm::vec3 position, glm::vec3 target, float speed)
    {
        this->position = position;
        this->speed = speed;
        forward = glm::normalize(target - position);

        up = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::normalize(glm::cross(forward, up));
        up = glm::cross(right, forward);
    };

    glm::mat4x4 getView()
    {
        return glm::lookAt(position, position + forward, up);
    }
};