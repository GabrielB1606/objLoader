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
uniform float explosionScale;

void main(){

    vec3 face_normal = cross( 
        vec3(gl_in[1].gl_Position - gl_in[0].gl_Position),
        vec3(gl_in[2].gl_Position - gl_in[0].gl_Position)
    );
    
    face_normal = normalize(face_normal);

    for(int i = 0; i<3; i++){
        
        gl_Position =
            ProjectionMatrix * ViewMatrix * ModelMatrix *
            (gl_in[i].gl_Position+ explosionScale*vec4(face_normal, 0.0f));
        
        vs_color = data_in[i].color;
        EmitVertex();
        
    }
    
    EndPrimitive();

}