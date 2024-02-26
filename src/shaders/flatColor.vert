#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 localPos = aPos + (aNormal * 0.1);
    gl_Position = (projection * view * model * vec4(localPos, 1.0));
}