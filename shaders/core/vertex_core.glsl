#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

#define M_PI 3.1415926535897932384626433832795

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    int type;
    
    vec3 direction;
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

vec3 calculateDiffuse(Material mtl, vec3 position, vec3 normal, Light light){
    
    vec3 lightDir = vec3(0.f);

    if( light.type == 0 )
        lightDir = normalize( light.position - position );
    else if(light.type == 1)
        lightDir = normalize( light.direction );

    float diffuse = max( dot(lightDir, normalize(normal)), 0.0 );

    return diffuse * mtl.diffuse;
}

vec3 calculateSpecular(Material mtl, vec3 position, vec3 normal, Light light, vec3 camPosition){

    vec3 lightToPosNorm = vec3(0.f);

    if( light.type == 0 )
        lightToPosNorm = normalize( position -  light.position );
    else if( light.type == 1 )
        lightToPosNorm = normalize( -light.direction );

    vec3 posToViewNorm = normalize( camPosition - position );
    vec3 reflectNorm = normalize( reflect( lightToPosNorm, normalize(normal) ) );
    float specularConstant = pow( max( dot( posToViewNorm, reflectNorm ), 0 ), mtl.shininess ); //35 must be the shininess of the mtl

    return mtl.specular * specularConstant;
}

float calculateAttenuation(Light light, vec3 position){

    float dist = length( light.position - position );
    return 1.f/ ( light.constant + (light.linear*dist) +  ( light.quadratic*dist*dist ) );

}

// input
    // world
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

    // context
uniform Material material;
uniform Light lights[3];
uniform int n_lights;
uniform vec3 camPosition;

uniform float normalFactor;
uniform int texCoordGen;

    // menu
uniform int gouraudShading;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;


// output
    // vertex info
out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} data_out;


void main(){
    
        // normal vertex shader stuff
    data_out.position =  vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    data_out.normal = normalize( vec4(ModelMatrix * vec4(vertex_normal, 1.f)).xyz );
    
    if( texCoordGen == 0 )
        data_out.texcoord = vec2( vertex_texcoord.x, vertex_texcoord.y* -1.f );
    else if( texCoordGen == 1 )
        data_out.texcoord = vec2( (normalFactor+vertex_position.x)/(2.f*normalFactor), (normalFactor+vertex_position.y)/(2.f*normalFactor) );
    else if( texCoordGen == 2 ){
        vec3 normal = normalize( vertex_normal );
        data_out.texcoord.x = atan( normal.x, normal.z )/(2*M_PI) + 0.5;
        data_out.texcoord.y = asin(normal.y)/(M_PI) + 0.5;
    }
    
    data_out.color = vertex_color;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
    
    data_out.ambient = vec3(0.f);
    data_out.diffuse = vec3(0.f);
    data_out.specular = vec3(0.f);
    
    float attenuation = 0.f;

        // if shading in this step
    if( gouraudShading != 0 ){
        
        for(int i = 0; i<n_lights; i++){

            if( lights[i].type == 0 )
                attenuation = calculateAttenuation(lights[i], data_out.position);
            else
                attenuation = 1.f;

            if( ambientLighting != 0 )
                data_out.ambient += calculateAmbient(material, lights[i].color, lights[i].intensity)*attenuation;

            if( diffuseLighting != 0 )
                data_out.diffuse += calculateDiffuse(material, data_out.position, data_out.normal, lights[i])*attenuation;
            
            if( specularLighting != 0 )
                data_out.specular += calculateSpecular(material, data_out.position, data_out.normal, lights[i], camPosition)*attenuation;
        }

    }

}