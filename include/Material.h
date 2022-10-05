#pragma once

class Material{
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    /* data */
public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Material();
    void sendToShader(Shader& programID);
};

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Material::~Material(){}

void Material::sendToShader(Shader& program){
    program.setVec3f(this->ambient, "material.ambient");
    program.setVec3f(this->diffuse, "material.diffuse");
    program.setVec3f(this->specular, "material.specular");
}