#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat3 modelNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 texPos;


const float density = 0.0015;
const float gradient = 5;

void main(){
    
    
    vec4 positionToCam = view * model * vec4(aPos, 1.0);
    
    gl_Position = projection * positionToCam;
        
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    Normal = modelNormal * aNormal;
    
    texPos = aTex;
    
}

