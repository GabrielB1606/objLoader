#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// in
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;
in vec3 vs_normal;

// out
out vec4 fs_color;

// uniforms
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 camPosition;

// functions
vec3 calculateAmbient(Material mtl){
    return mtl.ambient;
}

vec3 calculateDiffuse(Material mtl, vec3 position, vec3 normal, vec3 lightPos){
    
    vec3 posToLightNorm = normalize( lightPos - position );
    float diffuse = max( dot(normal, posToLightNorm), 0.f ) ;

    return mtl.diffuse * diffuse;
}

vec3 calculateSpecular(Material mtl, vec3 position, vec3 normal, vec3 lightPos, vec3 camPosition){
    
    vec3 lightToPosNorm = normalize(  position - lightPos );
    vec3 reflectNorm = normalize( reflect( lightToPosNorm, normal ) );
    vec3 posToViewNorm = normalize( position - camPosition );
    float specularConstant = pow( max( dot( posToViewNorm, reflectNorm ), 0 ), 20 );

    return mtl.specular * specularConstant;
}

void main(){

    // ambient light
    vec3 ambientFinal = calculateAmbient(material);

    // diffuse light
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);

    // specular light
    vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, lightPos0, camPosition);

    // attenuation

    vec3 lightFinal = ambientFinal + diffuseFinal + specularFinal;
    fs_color = vec4(vs_color, 1.f) * vec4( lightFinal, 1.0 ) /*+ vec4(specularFinal, 1.f)*/ ;
}