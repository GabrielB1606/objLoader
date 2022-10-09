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
    
    vec3 lightDir = normalize( lightPos - position );
    // float diffuse = max( dot(normal, lightDir), 0.0 ); 
    float diffuse = clamp( dot(lightDir, normal), 0, 1 ) ;

    vec3 ans = mtl.diffuse * diffuse;

    return ans;
}

vec3 calculateSpecular(Material mtl, vec3 position, vec3 normal, vec3 lightPos, vec3 camPosition){
    
    vec3 lightToPosNorm = normalize( position -  lightPos );
    vec3 reflectNorm = normalize( reflect( lightToPosNorm, normal ) );
    vec3 posToViewNorm = normalize( camPosition - position );
    float specularConstant = pow( max( dot( posToViewNorm, reflectNorm ), 0 ), 35 );

    return mtl.specular * specularConstant;
}

void main(){

    vec3 normal = normalize(vs_normal);

    // ambient light
    vec3 ambientFinal = calculateAmbient(material);

    // diffuse light
    vec3 diffuseFinal = calculateDiffuse(material, vs_position, normal, lightPos0);

    // specular light
    vec3 specularFinal = calculateSpecular(material, vs_position, normal, lightPos0, camPosition);

    // attenuation

    // vec3 lightFinal = ambientFinal + diffuseFinal /*+ specularFinal*/;
    // vec3 result = vs_color * lightFinal; 
    // fs_color = vec4(result, 1.0);
    fs_color = vec4(vs_color, 1.f);

	// fs_color = 	(vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}