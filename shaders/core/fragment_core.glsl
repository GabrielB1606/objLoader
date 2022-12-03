#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    float intensity;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};


// input
in Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_in;

    // menu
uniform int phongShading;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;

// output
out vec4 fs_color;

// uniforms
uniform Material material;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main(){

    // fs_color = vec4( material.diffuse , 1.f);
    if(phongShading != 0){
        fs_color = texture(tex0, data_in.texcoord);
    }else{
        fs_color = texture(tex0, data_in.texcoord) * vec4(data_in.color, 1.f);
    }

}