#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_textcoord;
layout (location = 3) in vec3 vertex_normal;

// out vec3 vs_position;
// out vec3 vs_color;
// out vec2 vs_textcoord;
// out vec3 vs_normal;

out DATA{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textcoord;
} data_out;

void main(){
    data_out.position = vertex_position;
    data_out.normal = vertex_normal;
    data_out.color = vertex_color;
    data_out.textcoord = vertex_textcoord;
    gl_Position = vec4(vertex_position, 1.f);

    // // vs_normal = mat3(ModelMatrix) * vertex_normal;
    // // vs_normal = mat3(transpose(inverse(ModelMatrix))) * vertex_normal;
}