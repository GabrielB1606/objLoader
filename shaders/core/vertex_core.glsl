#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_out;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){
    data_out.position =  vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    data_out.normal = vertex_normal;
    data_out.color = vertex_color;
    data_out.texcoord = vertex_texcoord;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);

    // // vs_normal = mat3(ModelMatrix) * vertex_normal;
    // // vs_normal = mat3(transpose(inverse(ModelMatrix))) * vertex_normal;
}