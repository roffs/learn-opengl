#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

// struct DirectionalLight {
//     vec3 direction;
  
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;  
uniform Material material;
uniform PointLight light;  

void main()
{
    vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));
    vec3 lightDir = normalize(light.position - FragPos);  

    // ambient light
    vec3 ambient = diffuseColor * light.ambient;
    
    // diffuse light
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * diffuseColor) * light.diffuse;

    // specular light - Blinn–Phong reflection model   
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

    vec3 specular = (spec * vec3(texture(material.specular, TexCoord))) * light.specular ;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));   

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}