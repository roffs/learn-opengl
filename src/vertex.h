#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex() {}
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) : Position(position), Normal(normal), TexCoords(texCoords) {}
};

#endif