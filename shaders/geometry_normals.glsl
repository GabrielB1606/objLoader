#version 330

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_textcoord;
out vec3 vs_normal;

in DATA{
    vec3 normal;
    vec3 color;
    vec2 textcoord;
    mat4 projection;
} data_in[];

void main(){

    for(int i = 0; i<3; i++){
        gl_Position = data_in[0].projection * gl_in[0].gl_Position;
        EmitVertex();
        gl_Position = data_in[0].projection * (gl_in[0].gl_Position + 0.01f * vec4(data_in[0].normal, 0.0f));
        EmitVertex();
        EndPrimitive();
    }
    

    // for(int i = 0; i<3; i++){
    //     gl_Position = data_in[i].projection * gl_in[i].gl_Position;

    //     vs_normal = data_in[i].normal;
    //     vs_color = data_in[i].color;
    //     vs_textcoord = data_in[i].textcoord;
        
    //     EmitVertex();
    // }
    
    // EndPrimitive();

}