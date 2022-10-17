#pragma once

class Model{
private:

    std::string name;

    Material* material;
    std::vector<Mesh*> meshes;
    glm::vec3 position;

    Mesh* boundingBox;

    float vertexSize = 10.f;

    void updateUniforms(Shader* shader);

public:
    Model(std::string name, std::vector<Mesh*> meshes, Material* material, glm::vec3 position, glm::vec3 rotation);
    ~Model();

    void update();
    void render(Shader* shader, bool showEdges, bool showVertices, bool wireframe );

    void rotate(const glm::vec3 rotation );
    void move(const glm::vec3 movement);
    void scaleUp(const glm::vec3 scale);

    void normalize(const float factor);
    void setBoundingBox( glm::vec3 maxComponents, glm::vec3 minComponents );

    Material* getMaterialReference();
    float* getVertexSizeReference();
    std::string getName();

};

void Model::setBoundingBox(glm::vec3 maxComponents, glm::vec3 minComponents){

    

}

void Model::normalize(const float factor){

    if(factor > 1)
        for(Mesh* &m: this->meshes)
            m->normalize(factor);

}

void Model::move(const glm::vec3 movement){
    for(Mesh* &m : this->meshes)
        m->move(movement);
    position += movement;
}

void Model::scaleUp(const glm::vec3 scale){
    for(Mesh* &m : this->meshes)
        m->scaleUp(scale);

}

std::string Model::getName(){
    return this->name;
}

float* Model::getVertexSizeReference(){
    return &this->vertexSize;
}

Material* Model::getMaterialReference(){
    return material;
}

void Model::rotate(const glm::vec3 rotation){
    for( Mesh* &m : meshes )
        m->rotate(rotation);
}

void Model::render(Shader* shader, bool showEdges = true, bool showVertices = false, bool fillOn = true){

    // this->updateUniforms(shader);
    if(fillOn){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonOffset(4.0, 1.0);
        material->sendToShader(*shader, GL_FILL);
        for(Mesh* m : meshes){
            m->render(shader);
        }
    }
    
    if( showEdges ){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonOffset(2.0, 1.0);
        material->sendToShader(*shader, GL_LINE);
        for(Mesh* m : meshes)
            m->render(shader);
        
    }

    if( showVertices ){
        glPointSize(this->vertexSize);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        material->sendToShader(*shader, GL_POINT);
        for(Mesh* m : meshes)
            m->render(shader);
        glPointSize(1.f);
        
    }
}

void Model::updateUniforms(Shader* shader){

    this->material->sendToShader(*shader);

}

void Model::update(){

}

Model::Model(std::string name, std::vector<Mesh*> meshes, Material* material, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f)){

    this->boundingBox = nullptr;
    this->name = name;
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
