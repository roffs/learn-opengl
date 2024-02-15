#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;

uniform float blueColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, texCoord) * vec4(vertexColor.xy, blueColor, 1.0);
}