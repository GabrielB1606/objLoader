#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_textcoord;
out vec3 vs_normal;

in DATA{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textcoord;
} data_in[];

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){

    for(int i = 0; i<3; i++){
        
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * gl_in[i].gl_Position;
        
        vs_position = vec4(ModelMatrix * vec4(data_in[i].position, 1.f)).xyz;
        vs_normal = data_in[i].normal;
        vs_color = data_in[i].color;
        vs_textcoord = data_in[i].textcoord;
        
        EmitVertex();
    }
    
    EndPrimitive();

}