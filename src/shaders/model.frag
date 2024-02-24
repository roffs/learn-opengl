#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
}; 


struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutOffCosine;
    float outerCutOffCosine;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  


in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;  
uniform Material material;

uniform DirectionalLight directionalLight; 
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];  
uniform SpotLight spotLight; 

uniform float shininess = 32;

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir);
    for(int i = 0; i < 4; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}


vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 diffuseColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * diffuseColor;
    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 diffuseColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * diffuseColor;
    vec3 diffuse  = light.diffuse  * diff * diffuseColor;
    vec3 specular = light.specular * spec * diffuseColor;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 diffuseColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 lightDir = normalize(light.position - fragPos);

    // ambient light
    vec3 ambient  = light.ambient  * diffuseColor;

     // diffuse light
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * diffuseColor) * light.diffuse;

    // specular light - Blinn–Phong reflection model   
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0), shininess);

    vec3 specular = (spec * vec3(texture(material.texture_specular1, TexCoords))) * light.specular ;

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));   // angle between current fragment and light direction
    float epsilon = (light.innerCutOffCosine - light.outerCutOffCosine);
    float intensity = clamp((theta - light.outerCutOffCosine) / epsilon, 0.0, 1.0);
    ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance));   

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
} 
