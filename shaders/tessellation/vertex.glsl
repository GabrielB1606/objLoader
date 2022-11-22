#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_textcoord;
layout (location = 3) in vec3 vertex_normal;

void main(){

    gl_Position = vec4(vertex_position, 1.f);

}