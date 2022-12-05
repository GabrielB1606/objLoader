#version 330

layout (triangles) in;  // get 3 vertices at the time
layout (triangle_strip, max_vertices = 3) out;  // send a max of 3 vertices per invocation, and take them as a triangle

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
    // vertex info
in Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_in[];

    // menu (do i have to shade in this step?)
uniform int flatShading;

    // context (light + mtl + menu ) info (from prev step)
in Context{
    Material material;
    PointLight pointLight;
    vec3 camPosition;
    int ambientLighting;
    int diffuseLighting;
    int specularLighting;
} context_in[];

// output
    // vertex info
out Vertex{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texcoord;
} data_out;

    // context (light + mtl + menu ) info (to pass forward)
out Context{
    Material material;
    PointLight pointLight;
    vec3 camPosition;
    int ambientLighting;
    int diffuseLighting;
    int specularLighting;
} context_out;


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
        vec3 color_out = vec3(0);

        if( context_in[0].ambientLighting != 0 )
            color_out += calculateAmbient(context_in[0].material, context_in[0].pointLight.color, context_in[0].pointLight.intensity);

        if( context_in[0].diffuseLighting != 0 )
            color_out += calculateDiffuse(context_in[0].material, samplePosition, faceNormal, context_in[0].pointLight.position);

        if( context_in[0].specularLighting != 0 )
            color_out += calculateSpecular(context_in[0].material, samplePosition, faceNormal, context_in[0].pointLight.position, context_in[0].camPosition);

            // build vertices for the next step
        for(int i = 0; i<3; i++){

            gl_Position = gl_in[i].gl_Position;
            
            data_out.position = data_in[i].position;
            data_out.normal = data_in[i].normal;
            data_out.texcoord = data_in[i].texcoord;

            data_out.color = color_out;

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
            
                // pass context data forward
            context_out.material = context_in[i].material;
            context_out.pointLight = context_in[i].pointLight;
            context_out.camPosition = context_in[i].camPosition;
            context_out.ambientLighting = context_in[i].ambientLighting;
            context_out.diffuseLighting = context_in[i].diffuseLighting;
            context_out.specularLighting = context_in[i].specularLighting;

            context_out.ambientLighting = 1;
            context_out.diffuseLighting = 1;
            context_out.specularLighting = 1;

            EmitVertex();
        }
        
        EndPrimitive();
    }


}