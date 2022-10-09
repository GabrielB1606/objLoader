#pragma once

class Model{
private:
    Material* material;
    std::vector<Mesh*> meshes;
    glm::vec3 position;

    void updateUniforms(Shader* shader);

public:
    Model(std::vector<Mesh*> meshes, Material* material, glm::vec3 position);
    Model(const char* objFile, Material* material, glm::vec3 position);
    ~Model();

    void update();
    void render(Shader* shader);

    void rotate(const glm::vec3 rotation );

};

void Model::rotate(const glm::vec3 rotation){
    for( Mesh* &m : meshes )
        m->rotate(rotation);
}

void Model::render(Shader* shader){

    this->updateUniforms(shader);

    for(Mesh* m : meshes)
        m->render(shader);

}

void Model::updateUniforms(Shader* shader){

    this->material->sendToShader(*shader);

}

void Model::update(){

}

Model::Model(const char* objFile, Material* material, glm::vec3 position = glm::vec3(0.f)){

    this->position = position;
    this->material = material;

    std::vector<Vertex> temp = LoadOBJ("../../obj/cube.obj");

    this->meshes.push_back( new Mesh( temp.data(), temp.size() ) );
    

    for(Mesh* &m:meshes){
        m->move(this->position);
        m->setOrigin(this->position);
    }

}

Model::Model(std::vector<Mesh*> meshes, Material* material, glm::vec3 position = glm::vec3(0.f)){

    this->position = position;
    this->material = material;

    for( Mesh* m : meshes ){
        this->meshes.push_back( new Mesh( *m ) );
    }

    for(Mesh* &m:meshes){
        m->move(this->position);
        m->setOrigin(this->position);
    }

}

Model::~Model(){
    for( Mesh* &m : this->meshes )
        delete m;
}
