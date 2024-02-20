#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include <glm/glm.hpp>

struct PointLight
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 position,
               glm::vec3 ambient,
               glm::vec3 diffuse,
               glm::vec3 specular,
               float constant,
               float linear,
               float quadratic) : position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic) {}
};

#endif