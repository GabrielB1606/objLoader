#pragma once

class Material{
private:

    std::string name;

    glm::vec3 fill;
    glm::vec3 edge;
    glm::vec3 vertex;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess = 20.f;

    Texture *map_kd, *map_ka, *map_ks;

public:
    Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, Texture *map_ka, Texture *map_kd , Texture *map_ks);
    virtual ~Material();
    void sendToShader(Shader& programID, GLenum type);

    glm::vec3* getFillColorReference();
    glm::vec3* getVertexColorReference();
    glm::vec3* getEdgeColorReference();

    std::string getName();

    void assignTexture( unsigned int type, Texture* tex );
    void assignTexture( unsigned int type, std::string filename );

    void deactivate();

    static size_t default_counter;
};


void Material::deactivate(){

    // glActiveTexture(GL_TEXTURE0);
    // map_ka->unbind();
    // glActiveTexture(GL_TEXTURE1);
    // map_kd->unbind();
    // glActiveTexture(GL_TEXTURE2);
    // map_ks->unbind();

}

void Material::assignTexture(unsigned int type, Texture* tex){
    switch (type){
        case AMBIENT_MAP:
            map_ka = tex;
            break;
        case DIFFUSE_MAP:
            map_kd = tex;
            break;
        case SPECULAR_MAP:
            map_ks = tex;
            break;
        
        default:
            break;
    }
}

void Material::assignTexture(unsigned int type, std::string filename){
    switch (type){
        case AMBIENT_MAP:
            map_ka = new Texture(filename, GL_TEXTURE_2D);
            break;
        case DIFFUSE_MAP:
            map_kd = new Texture(filename, GL_TEXTURE_2D);
            break;
        case SPECULAR_MAP:
            map_ks = new Texture(filename, GL_TEXTURE_2D);
            break;
        
        default:
            break;
    }
}

glm::vec3* Material::getFillColorReference(){
    return &fill;
}

glm::vec3* Material::getVertexColorReference(){
    return &vertex;
}

glm::vec3* Material::getEdgeColorReference(){
    return &edge;
}

Material::Material(std::string name, glm::vec3 ambient = glm::vec3(0.7f), glm::vec3 diffuse = glm::vec3(0.9f), glm::vec3 specular = glm::vec3(1.f), float shininess = 20.f, Texture *map_ka = nullptr, Texture *map_kd = nullptr, Texture *map_ks = nullptr){
    this->name = name;
    this->fill = diffuse;
    this->vertex = glm::vec3(0.f, 1.f, 0.f);
    this->edge = glm::vec3(0.f);
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->map_ka = map_ka;
    this->map_kd = map_kd;
    this->map_ks = map_ks;
    this->shininess = shininess;
}

Material::~Material(){

    if( map_ka != nullptr )
        delete map_ka;
    
    if( map_ks != nullptr )
        delete map_ks;
    
    if( map_kd != nullptr )
        delete map_kd;

}

std::string Material::getName(){
    return this->name;
}

void Material::sendToShader(Shader& program, GLenum type = GL_FILL){

    program.setVec3f(this->ambient, "material.ambient");
    
    switch (type){
    case GL_FILL:
        
        if(map_ka != nullptr){
            program.set1i( AMBIENT_MAP, "map_ka" );
            program.set1i( 1, "mapKaPresent" );
            map_ka->bind( AMBIENT_MAP );
        }else
            program.set1i( 0, "mapKaPresent" );
        
        if(map_kd != nullptr){
            program.set1i( DIFFUSE_MAP, "map_kd" );
            program.set1i( 1, "mapKdPresent" );
            map_kd->bind( DIFFUSE_MAP );
        }else
            program.set1i( 0, "mapKdPresent" );

        if(map_ks != nullptr){
            program.set1i( SPECULAR_MAP, "map_ks" );
            program.set1i( 1, "mapKsPresent" );
            map_ks->bind( SPECULAR_MAP );
        }else
            program.set1i( 0, "mapKsPresent" );

        program.setVec3f(this->fill, "material.diffuse");
        program.set1f(this->shininess, "material.shininess");
        break;
    case GL_LINE:
        program.setVec3f(this->edge, "material.diffuse");
        break;
    case GL_POINT:
        program.setVec3f(this->vertex, "material.diffuse");
        break;
    
    default:
        break;
    }

    program.setVec3f(this->specular, "material.specular");
}

