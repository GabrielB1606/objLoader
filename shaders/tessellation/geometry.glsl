#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(){

    vec3 face_normal = cross( 
        vec3(gl_in[1].gl_Position - gl_in[0].gl_Position),
        vec3(gl_in[2].gl_Position - gl_in[0].gl_Position)
    );
    
    face_normal = normalize(face_normal);

    for(int i = 0; i<3; i++){
        
        // gl_Position =
        //     ProjectionMatrix * ViewMatrix * ModelMatrix *
        //     (gl_in[i].gl_Position + 0.01*face_normal);
        
        gl_Position =
            ProjectionMatrix * ViewMatrix * ModelMatrix *
            (gl_in[i].gl_Position);
        
        EmitVertex();
    }
    
    EndPrimitive();

}