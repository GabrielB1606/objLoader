#version 330

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec4 PickingColor;

void main(){

    color = PickingColor;

}