#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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
} data_in[];

    // menu
uniform int flatShading;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;

// output
out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_out;

void main(){

    for(int i = 0; i<3; i++){
        
        gl_Position = gl_in[i].gl_Position;
        
        data_out.position = data_in[i].position;
        data_out.normal = data_in[i].normal;
        data_out.color = data_in[i].color;
        data_out.texcoord = data_in[i].texcoord;
        
        EmitVertex();
    }
    
    EndPrimitive();

}