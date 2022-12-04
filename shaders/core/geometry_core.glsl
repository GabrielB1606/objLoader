#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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
} data_in[];

    // menu
uniform int flatShading;

in Context{
    Material material;
    PointLight pointLight;
    vec3 camPosition;
    int ambientLighting;
    int diffuseLighting;
    int specularLighting;
} context_in[];

// output
out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_out;

out Context{
    Material material;
    PointLight pointLight;
    vec3 camPosition;
    int ambientLighting;
    int diffuseLighting;
    int specularLighting;
} context_out;


void main(){


    if( flatShading != 0 ){

        // vec3 samplePosition = (data_in[0].position + data_in[1].position + data_in[2].position)/3.f ;
        // vec3 samplePosition = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz  )/3 ;

        vec3 samplePosition = vec3(0.f);
        vec3 sampleNormal = vec3(0.f);
        for(int i = 0; i<3; i++){
            samplePosition += data_in[i].position;
            sampleNormal += data_in[i].normal;
        }
        sampleNormal = normalize( sampleNormal );
        samplePosition /= 3.f;

        vec3 color_out = vec3(0);

        if( context_in[0].ambientLighting != 0 )
            color_out += calculateAmbient(context_in[0].material, context_in[0].pointLight.color, context_in[0].pointLight.intensity);
        if( context_in[0].diffuseLighting != 0 )
            color_out += calculateDiffuse(context_in[0].material, samplePosition, sampleNormal, context_in[0].pointLight.position);
        if( context_in[0].specularLighting != 0 )
            color_out += calculateSpecular(context_in[0].material, samplePosition, sampleNormal, context_in[0].pointLight.position, context_in[0].camPosition);

        for(int i = 0; i<3; i++){
            gl_Position = gl_in[i].gl_Position;
            
            data_out.position = data_in[i].position;
            data_out.normal = data_in[i].normal;
            data_out.color = color_out;
            data_out.texcoord = data_in[i].texcoord;

            EmitVertex();

        }

        EndPrimitive();

    }else{

        for(int i = 0; i<3; i++){
            
            gl_Position = gl_in[i].gl_Position;
            
            data_out.position = data_in[i].position;
            data_out.normal = data_in[i].normal;
            data_out.color = data_in[i].color;
            data_out.texcoord = data_in[i].texcoord;
            
            context_out.material = context_in[i].material;
            context_out.pointLight = context_in[i].pointLight;
            context_out.camPosition = context_in[i].camPosition;
            context_out.ambientLighting = context_in[0].ambientLighting;
            context_out.diffuseLighting = context_in[0].diffuseLighting;
            context_out.specularLighting = context_in[0].specularLighting;

            EmitVertex();
        }
        
        EndPrimitive();
    }


}