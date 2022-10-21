#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// out
out vec4 fs_color;

uniform vec4 PickingColor;

void main(){

    fs_color = PickingColor;

}