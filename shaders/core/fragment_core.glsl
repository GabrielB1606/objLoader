#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// in
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;
in vec3 vs_normal;

// out
out vec4 fs_color;

// uniforms
uniform Material material;

void main(){

    fs_color = vec4( material.diffuse , 1.f);

}