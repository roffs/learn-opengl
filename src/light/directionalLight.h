#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>

struct DirectionalLight
{
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(glm::vec3 direction,
                     glm::vec3 ambient,
                     glm::vec3 diffuse,
                     glm::vec3 specular) : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {}
};

#endif