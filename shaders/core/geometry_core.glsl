#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

in Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_in[];

void main(){

    for(int i = 0; i<3; i++){
        
        gl_Position = gl_in[i].gl_Position;
        
        vs_position = data_in[i].position;
        vs_normal = data_in[i].normal;
        vs_color = data_in[i].color;
        vs_texcoord = data_in[i].texcoord;
        
        EmitVertex();
    }
    
    EndPrimitive();

}