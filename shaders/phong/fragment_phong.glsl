#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    float intensity;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

// in
in DATA{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 textcoord;
} data_in;

// out
out vec4 fs_color;

// uniforms
uniform Material material;
uniform PointLight pointLight;
uniform vec3 camPosition;

// functions
vec3 calculateAmbient(Material mtl, vec3 lightColor, float intensity){
    return mtl.ambient * lightColor * intensity;
}

vec3 calculateDiffuse(Material mtl, vec3 position, vec3 normal, vec3 lightPos){
    
    vec3 lightDir = normalize( lightPos - position );
    float diffuse = max( dot(lightDir, normalize(normal)), 0.0 );

    return diffuse * mtl.diffuse;
}

vec3 calculateSpecular(Material mtl, vec3 position, vec3 normal, vec3 lightPos, vec3 camPosition){
    
    vec3 lightToPosNorm = normalize( position -  lightPos );
    vec3 reflectNorm = normalize( reflect( lightToPosNorm, normalize(normal) ) );
    vec3 posToViewNorm = normalize( camPosition - position );
    float specularConstant = pow( max( dot( posToViewNorm, reflectNorm ), 0 ), 35 );

    return mtl.specular * specularConstant;
}

void main(){

    // attenuation
    // float distance = length(pointLight.position - vs_position);
    // float attenuation = 1.f/(pointLight.constant +( pointLight.linear * distance) + (pointLight.quadratic * distance * distance));
    
    // ambient light
    vec3 ambientFinal = calculateAmbient(material, pointLight.color, pointLight.intensity);

    // // diffuse light
    vec3 diffuseFinal = calculateDiffuse(material, data_in.position, data_in.normal, pointLight.position);

    // // specular light
    vec3 specularFinal = calculateSpecular(material, data_in.position, data_in.normal, pointLight.position, camPosition);

    vec3 lightFinal = ambientFinal + diffuseFinal + specularFinal;
    vec3 result = data_in.color * lightFinal; 
    fs_color = vec4(result, 1.f);

}