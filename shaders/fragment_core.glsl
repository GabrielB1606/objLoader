#version 440

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform vec3 lightPos0;
uniform vec3 camPosition;

void main(){

    // ambient light
    vec3 ambientLight = material.ambient;

    // diffuse light
    vec3 posToLightNorm = normalize( lightPos0 - vs_position );
    vec3 diffuseColor = vec3( 1.f, 1.f, 1.f );
    float diffuse = max( dot(vs_normal, posToLightNorm), 0.f ) ;
    vec3 diffuseFinal = material.diffuse * diffuse;

    // specular light
    vec3 lightToPosNorm = normalize(  vs_position - lightPos0 );
    vec3 reflectNorm = normalize( reflect( lightToPosNorm, vs_normal ) );
    vec3 posToViewNorm = normalize( vs_position - camPosition );
    float specularConstant = pow( max( dot( posToViewNorm, reflectNorm ), 0 ), 20 );
    vec3 specularFinal = material.specular * specularConstant;

    // attenuation

    vec3 lightFinal = ambientLight + diffuseFinal + specularFinal;
    fs_color = vec4(vs_color, 1.f) * vec4( lightFinal, 1.0 ) /*+ vec4(specularFinal, 1.f)*/ ;
}