#version 460 core
#extension GL_ARB_tessellation_shader : enable

layout (vertices = 3) out;

uniform float innerTess;
uniform float outerTess;

void main(void){
    gl_TessLevelOuter[0] = outerTess;
    gl_TessLevelOuter[1] = outerTess;
    gl_TessLevelOuter[2] = outerTess;

    
    gl_TessLevelInner[0] = innerTess;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}