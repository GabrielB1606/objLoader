#version 440

// out
out vec4 fs_color;

uniform vec4 PickingColor;

void main(){

    fs_color = PickingColor;

}