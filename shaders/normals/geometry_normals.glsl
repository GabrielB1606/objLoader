#version 330

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} data_in[];

out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} data_out;

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