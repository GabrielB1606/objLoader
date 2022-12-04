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

public:
    Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Material();
    void sendToShader(Shader& programID, GLenum type);

    glm::vec3* getFillColorReference();
    glm::vec3* getVertexColorReference();
    glm::vec3* getEdgeColorReference();

    std::string getName();

    static size_t default_counter;
};


glm::vec3* Material::getFillColorReference(){
    return &fill;
}

glm::vec3* Material::getVertexColorReference(){
    return &vertex;
}

glm::vec3* Material::getEdgeColorReference(){
    return &edge;
}

Material::Material(std::string name, glm::vec3 ambient = glm::vec3(0.7f), glm::vec3 diffuse = glm::vec3(0.9f), glm::vec3 specular = glm::vec3(1.f)){
    this->name = name;
    this->fill = diffuse;
    this->vertex = glm::vec3(0.f, 1.f, 0.f);
    this->edge = glm::vec3(0.f);
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Material::~Material(){}

std::string Material::getName(){
    return this->name;
}

void Material::sendToShader(Shader& program, GLenum type = GL_FILL){

    program.setVec3f(this->ambient, "material.ambient");
    
    switch (type){
    case GL_FILL:
        program.setVec3f(this->fill, "material.diffuse");
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