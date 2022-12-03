#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

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
    // world
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

    // context
uniform Material material;
uniform PointLight pointLight;
uniform vec3 camPosition;

    // menu
uniform int gouraudShading;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;


// output
out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_out;

void main(){
    
    data_out.position =  vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
    data_out.normal = normalize( vec4(ModelMatrix * vec4(vertex_normal, 1.f)).xyz );
    data_out.texcoord = vec2( vertex_texcoord.x, vertex_texcoord.y* -1.f );
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
    
    if( gouraudShading != 0 ){
        data_out.color = vec3(0);

        if( ambientLighting != 0 )
            data_out.color += calculateAmbient(material, pointLight.color, pointLight.intensity);
        if( diffuseLighting != 0 )
            data_out.color += calculateDiffuse(material, data_out.position, data_out.normal, pointLight.position);
        if( specularLighting != 0 )
            data_out.color += calculateSpecular(material, data_out.position, data_out.normal,pointLight.position, camPosition);

    }else{
        data_out.color = vertex_color;
    }


    // // vs_normal = mat3(ModelMatrix) * vertex_normal;
    // // vs_normal = mat3(transpose(inverse(ModelMatrix))) * vertex_normal;
}