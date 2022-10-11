#pragma once

class Material{
private:
    glm::vec3 fill;
    glm::vec3 vertex;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    /* data */
public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Material();
    void sendToShader(Shader& programID, bool fillMode);

    glm::vec3* getFillColorReference();
    glm::vec3* getVertexColorReference();
};


glm::vec3* Material::getFillColorReference(){
    return &fill;
}

glm::vec3* Material::getVertexColorReference(){
    return &vertex;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
    this->fill = diffuse;
    this->vertex = glm::vec3(0.f);
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Material::~Material(){}

void Material::sendToShader(Shader& program, bool fillMode = true){
    program.setVec3f(this->ambient, "material.ambient");
    
    if(fillMode)
        program.setVec3f(this->fill, "material.diffuse");
    else
        program.setVec3f(this->vertex, "material.diffuse");

    program.setVec3f(this->specular, "material.specular");
}