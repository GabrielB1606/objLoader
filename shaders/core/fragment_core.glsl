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
uniform Material material;
uniform PointLight pointLight;
uniform vec3 camPosition;

    // menu
uniform int phongShading;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;

    // textures
uniform sampler2D tex0;
uniform sampler2D tex1;

// output
out vec4 fs_color;

void main(){

        // if shading in this step
    if( phongShading != 0 ){

        vec3 lightsFinal = vec3(0);

        if( ambientLighting != 0 )
            lightsFinal += calculateAmbient(material, pointLight.color, pointLight.intensity);
        if( diffuseLighting != 0 )
            lightsFinal += calculateDiffuse(material, data_in.position, data_in.normal, pointLight.position);
        if( specularLighting != 0 )
            lightsFinal += calculateSpecular(material, data_in.position, data_in.normal, pointLight.position, camPosition);

        fs_color = texture(tex0, data_in.texcoord) * vec4(lightsFinal, 1.f);
        
    }else{

            // if I'm not shading in this step, then someone before me shaded and saved the light stuff in the color component 
        fs_color = texture(tex0, data_in.texcoord) * vec4(data_in.color, 1.f);
        
    }


}