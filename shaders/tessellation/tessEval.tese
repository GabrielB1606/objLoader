#version 460 core
#extension GL_ARB_tessellation_shader : enable

layout (triangles, equal_spacing, ccw) in;

void main(){

    vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;

    gl_Position = vec4(normalize(p0 + p1 + p2), 1.0);

    // float r = 0.850651;

    // gl_Position.x = mix(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x, gl_TessCoord.x);
    // gl_Position.y = mix(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y, gl_TessCoord.y);
    // gl_Position.z = mix(gl_in[0].gl_Position.z, gl_in[1].gl_Position.z, gl_TessCoord.z);

    // gl_Position.x = mix(gl_Position.x, gl_in[2].gl_Position.x, gl_TessCoord.x);
    // gl_Position.y = mix(gl_Position.y, gl_in[2].gl_Position.y, gl_TessCoord.y);
    // gl_Position.z = mix(gl_Position.z, gl_in[2].gl_Position.z, gl_TessCoord.z);

}