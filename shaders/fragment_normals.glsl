#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// out
out vec4 fs_color;

void main(){

    fs_color = vec4( 0.8f, 0.3f, 0.02f , 1.f);

}