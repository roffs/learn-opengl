#ifndef FLAT_MATERIAL_H
#define FLAT_MATERIAL_H
#include <glm/glm.hpp>

struct FlatMaterial
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    FlatMaterial(glm::vec3 ambient,
                 glm::vec3 diffuse,
                 glm::vec3 specular,
                 float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
};

#endif