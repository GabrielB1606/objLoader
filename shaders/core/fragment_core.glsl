#version 440

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // sampler2D map_ka;
    // sampler2D map_kd;
    // sampler2D map_ks;
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

uniform int mapKaPresent;
uniform int mapKsPresent;
uniform int mapKdPresent;

    // textures
// uniform sampler2D texture_map;
uniform sampler2D map_ka;
uniform sampler2D map_kd;
uniform sampler2D map_ks;

// output
out vec4 fs_color;

void main(){

    vec3 specular = vec3(0);
    vec3 ambient = vec3(0);
    vec3 diffuse = vec3(0);

        // if shading in this step
    if( phongShading != 0 ){


        if( ambientLighting != 0 )
            ambient = calculateAmbient(material, pointLight.color, pointLight.intensity);
        if( diffuseLighting != 0 )
            diffuse = calculateDiffuse(material, data_in.position, data_in.normal, pointLight.position);
        if( specularLighting != 0 )
            specular = calculateSpecular(material, data_in.position, data_in.normal, pointLight.position, camPosition);

        // if( map_kd != -1 )
            // fs_color = texture( map_kd, data_in.texcoord) * vec4(lightsFinal, 1.f);
        // else
        //     fs_color = material.diffuse * vec4(lightsFinal, 1.f);
        
    }else{

            // if I'm not shading in this step, then someone before me shaded and saved the light stuff in the color component 
        // if( map_kd != -1 )
        // else
        //     fs_color = material.diffuse * vec4(data_in.color, 1.f);
        specular = data_in.specular;
        ambient = data_in.ambient;
        diffuse = data_in.diffuse;
    }

    if( specularLighting + mapKsPresent > 0 )
        specular *= texture( map_ks, data_in.texcoord).xyz ;

    if( diffuseLighting + mapKdPresent > 0 )
        diffuse *= texture( map_kd, data_in.texcoord).xyz;

    if( ambientLighting + mapKaPresent > 0 )
        ambient *= texture( map_ka, data_in.texcoord).xyz;    

    vec3 lightsFinal = specular + ambient + diffuse;

    if(lightsFinal != vec3(0))
        fs_color = texture( map_kd, data_in.texcoord) * vec4( lightsFinal , 1.f);
    else
        fs_color = texture( map_kd, data_in.texcoord);

}