#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;  
uniform Material material;
uniform Light light;  

void main()
{
    // ambient light
    vec3 ambient = material.ambient * light.ambient;
    
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // specular light - Blinn–Phong reflection model   
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

    vec3 specular = (spec * material.specular) * light.specular ;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}