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

// input
in Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_in;

    // context
in Context{
    Material material;
    PointLight pointLight;
    vec3 camPosition;
    int ambientLighting;
    int diffuseLighting;
    int specularLighting;
} context_in;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;

    // menu
uniform int phongShading;

    // textures
uniform sampler2D tex0;
uniform sampler2D tex1;

// output
out vec4 fs_color;

void main(){

    if( phongShading != 0 ){
        vec3 lightsFinal = vec3(0);

        if( ambientLighting != 0 )
            lightsFinal += calculateAmbient(context_in.material, context_in.pointLight.color, context_in.pointLight.intensity);
        if( diffuseLighting != 0 )
            lightsFinal += calculateDiffuse(context_in.material, data_in.position, data_in.normal, context_in.pointLight.position);
        if( specularLighting != 0 )
            lightsFinal += calculateSpecular(context_in.material, data_in.position, data_in.normal, context_in.pointLight.position, context_in.camPosition);

        fs_color = texture(tex0, data_in.texcoord) * vec4(lightsFinal, 1.f);
        
    }else{
        fs_color = texture(tex0, data_in.texcoord) * vec4(data_in.color, 1.f);
    }


}