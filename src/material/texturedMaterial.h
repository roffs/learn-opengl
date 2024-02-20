#ifndef TEXTURED_MATERIAL_H
#define TEXTURED_MATERIAL_H
#include <glm/glm.hpp>

#include "../texture.h"

struct TexturedMaterial
{
    Texture diffuse;
    Texture specular;
    float shininess;

    TexturedMaterial(Texture diffuse,
                     Texture specular,
                     float shininess) : diffuse(diffuse), specular(specular), shininess(shininess) {}
};

#endif