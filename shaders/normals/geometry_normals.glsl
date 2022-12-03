#version 330

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

in DATA{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_in[];

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){

    for(int i = 0; i<3; i++){
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * (gl_in[i].gl_Position + 0.025*vec4(data_in[i].normal, 0.0f));
        EmitVertex();
        EndPrimitive();
    }

    // gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    // EmitVertex();
    // gl_Position = data_in[0].projection * (gl_in[0].gl_Position + vec4(data_in[0].normal, 0.0f));
    // EmitVertex();
    // EndPrimitive();
    
    // gl_Position = data_in[1].projection * gl_in[1].gl_Position;
    // EmitVertex();
    // gl_Position = data_in[1].projection * (gl_in[1].gl_Position + vec4(data_in[1].normal, 0.0f));
    // EmitVertex();
    // EndPrimitive();
    
    // gl_Position = data_in[2].projection * gl_in[2].gl_Position;
    // EmitVertex();
    // gl_Position = data_in[2].projection * (gl_in[2].gl_Position + vec4(data_in[2].normal, 0.0f));
    // EmitVertex();
    // EndPrimitive();

}