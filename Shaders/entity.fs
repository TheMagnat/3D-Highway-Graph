#version 330 core


struct Material {
    //Couleur des reflet sur chaque type de reflets
    //vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float shininess;
};
uniform Material material;

struct Light {
    //Position de la lumiere
    vec3 position;
    vec3 direction;
    
    //Intensité de la lumière sur chaque type de reflet
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    //Attenuation
    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform vec3 viewPos;

//Venant du vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 texPos;

in float visibility;


//Couleur de sortie du pixel
out vec4 FragColor;


void main(){
    
    vec3 colorAmbient = material.diffuse;
    vec3 colorDiffuse = material.diffuse;
    vec3 colorSpecular = material.specular;
    
    // ambient
    vec3 ambient = light.ambient * colorAmbient;
    // diffuse
    
    //Ici c'est la normal normalement..
    vec3 norm = normalize(Normal);
    
    //vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * colorDiffuse;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * colorSpecular;

    
    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
    
    //FragColor = mix(vec4(0.52f, 0.83f, 1.0f, 1.0f), FragColor, visibility);
    
    
}


