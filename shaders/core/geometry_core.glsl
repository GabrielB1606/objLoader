#version 330

layout (triangles) in;  // get 3 vertices at the time
layout (triangle_strip, max_vertices = 3) out;  // send a max of 3 vertices per invocation, and take them as a triangle

// structs
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light{
    int type;
    
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
    // vertex info
in Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} data_in[];

    // menu (do i have to shade in this step?)
uniform int flatShading;
uniform int ambientLighting;
uniform int diffuseLighting;
uniform int specularLighting;

    // context
uniform Material material;
uniform Light lights[3];
uniform int n_lights;
uniform vec3 camPosition;

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

        // if shading in this step
    if( flatShading != 0 ){

        vec3 samplePosition = vec3(0.f);    // get the center of the primitive as the sample point for the lighting
        for(int i = 0; i<3; i++){
            samplePosition += data_in[i].position;
        }
        samplePosition /= 3.f;


        vec3 faceNormal =   // we also need the normal of the face
            normalize(
                cross( 
                    vec3(data_in[1].position - data_in[0].position),
                    vec3(data_in[2].position - data_in[0].position)
                )
            );

            // accumulate light stuff in color_out to pass it to the next step
        vec3 specular_out = vec3(0);
        vec3 diffuse_out = vec3(0);
        vec3 ambient_out = vec3(0);

        for(int i = 0; i<n_lights; i++){

            if( ambientLighting != 0 )
                ambient_out += calculateAmbient(material, lights[i].color, lights[i].intensity);

            if( diffuseLighting != 0 )
                diffuse_out += calculateDiffuse(material, samplePosition, faceNormal, lights[i].position);

            if( specularLighting != 0 )
                specular_out += calculateSpecular(material, samplePosition, faceNormal, lights[i].position, camPosition);

        }


            // build vertices for the next step
        for(int i = 0; i<3; i++){

            gl_Position = gl_in[i].gl_Position;
            
            data_out.position = data_in[i].position;
            data_out.normal = data_in[i].normal;
            data_out.texcoord = data_in[i].texcoord;
            data_out.color = data_in[i].color;

            data_out.specular = specular_out;
            data_out.ambient = ambient_out;
            data_out.diffuse = diffuse_out;

            EmitVertex();

        }

        EndPrimitive();

    }else{  // if not shading in this step

        for(int i = 0; i<3; i++){

                // pass vertex data forward
            gl_Position = gl_in[i].gl_Position;
            
            data_out.position = data_in[i].position;
            data_out.normal = data_in[i].normal;
            data_out.color = data_in[i].color;
            data_out.texcoord = data_in[i].texcoord;

            data_out.specular = data_in[i].specular;
            data_out.ambient =  data_in[i].ambient;
            data_out.diffuse =  data_in[i].diffuse;

            EmitVertex();
        }
        
        EndPrimitive();
    }


}