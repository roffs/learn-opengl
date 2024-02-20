#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H
#include <glm/glm.hpp>

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    float innerCutOffAngle;
    float outerCutOffAngle;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    SpotLight(
        glm::vec3 position,
        glm::vec3 direction,
        float innerCutOffAngle,
        float outerCutOffAngle,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        float constant,
        float linear,
        float quadratic) : direction(direction),
                           position(position),
                           innerCutOffAngle(innerCutOffAngle), outerCutOffAngle(outerCutOffAngle),
                           ambient(ambient), diffuse(diffuse), specular(specular),
                           constant(constant), linear(linear), quadratic(quadratic)
    {
    }
};

#endif