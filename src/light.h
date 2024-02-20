#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>

struct Light
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(glm::vec3 position,
          glm::vec3 ambient,
          glm::vec3 diffuse,
          glm::vec3 specular) : position(position), ambient(ambient), diffuse(diffuse), specular(specular) {}
};

#endif