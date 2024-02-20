#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular,
             float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
};

#endif