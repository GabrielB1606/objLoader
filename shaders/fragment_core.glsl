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
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;
in vec3 vs_normal;

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
    // float diffuse = max( dot(normal, lightDir), 0.0 ); 
    float diffuse = clamp( dot(lightDir, normalize(normal)), 0, 1 ) ;

    vec3 ans = mtl.diffuse * diffuse;

    return ans;
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
    float distance = length(pointLight.position - vs_position);
    float attenuation = 1.f/(pointLight.constant +( pointLight.linear * distance) + (pointLight.quadratic * distance * distance));
    
    // ambient light
    vec3 ambientFinal = calculateAmbient(material, pointLight.color, pointLight.intensity) * attenuation;

    // diffuse light
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position) * attenuation;

    // specular light
    vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, camPosition) * attenuation;


    vec4 lightFinal = vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) /*+ specularFinal*/;
    // fs_color = lightFinal;
    vec3 result = vs_color * lightFinal; 
    // fs_color = vec4(result, 1.0);
    // fs_color = vec4( material.diffuse , 1.f);

	// fs_color = 	(vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}