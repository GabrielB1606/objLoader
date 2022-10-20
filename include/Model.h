#pragma once

class Model{
private:

    std::string name;

    std::vector<Mesh*> meshes;
    glm::vec3 position;

    Mesh* boundingBox = nullptr;

    float vertexSize = 10.f;

public:
    Model(std::string name, std::vector<Mesh*> meshes, glm::vec3 position, glm::vec3 rotation);
    ~Model();

    void update();
    void render(Shader* shader, bool showEdges, bool showVertices, bool wireframe, bool showBoundingBox, bool prevCull );

    void rotate(const glm::vec3 rotation );
    void move(const glm::vec3 movement);
    void scaleUp(const glm::vec3 scale);

    void normalize(const float factor);
    void setBoundingBox( glm::vec3 maxComponents, glm::vec3 minComponents );

    float* getVertexSizeReference();
    std::string getName();

};

void Model::setBoundingBox(glm::vec3 maxComponents, glm::vec3 minComponents){

    Vertex vertices[] = {
        Vertex( glm::vec3(  minComponents.x,    minComponents.y,    minComponents.z)),
        Vertex( glm::vec3(  maxComponents.x,    minComponents.y,    minComponents.z)),
        Vertex( glm::vec3(  maxComponents.x,    maxComponents.y,    minComponents.z)),
        Vertex( glm::vec3(  minComponents.x,    maxComponents.y,    minComponents.z)),

        Vertex( glm::vec3(  minComponents.x,    minComponents.y,    maxComponents.z)),
        Vertex( glm::vec3(  maxComponents.x,    minComponents.y,    maxComponents.z)),
        Vertex( glm::vec3(  maxComponents.x,    maxComponents.y,    maxComponents.z)),
        Vertex( glm::vec3(  minComponents.x,    maxComponents.y,    maxComponents.z)),
    };  

    GLuint indices[] = {
        0, 2, 1,
        3, 2, 0,

        6, 1, 2,
        6, 5, 1,
        
        7, 3, 0,
        0, 4, 7,
        
        7, 5, 6,
        7, 4, 5,

        3, 6, 2,
        3, 7, 6,

        0, 1, 5,
        0, 5, 4
    };

    boundingBox = new Mesh( vertices, 8, GL_TRIANGLES, indices, 36);

}

void Model::normalize(const float factor){

    if(factor > 1){
        for(Mesh* &m: this->meshes)
            m->normalize(factor);

        if( boundingBox != nullptr )
            boundingBox->normalize(factor);
    }

}

void Model::move(const glm::vec3 movement){
    for(Mesh* &m : this->meshes)
        m->move(movement);
    boundingBox->move(movement);
    position += movement;
}

void Model::scaleUp(const glm::vec3 scale){
    for(Mesh* &m : this->meshes)
        m->scaleUp(scale);
    boundingBox->scaleUp(scale);
}

std::string Model::getName(){
    return this->name;
}

float* Model::getVertexSizeReference(){
    return &this->vertexSize;
}

void Model::rotate(const glm::vec3 rotation){
    for( Mesh* &m : meshes )
        m->rotate(rotation);
    boundingBox->rotate(rotation);
}

void Model::render(Shader* shader, bool showEdges = true, bool showVertices = false, bool fillOn = true, bool showBoundingBox = false, bool prevCull = true){

    // this->updateUniforms(shader);

    for(Mesh* m : meshes){
        m->render(shader, fillOn, showEdges, showVertices, vertexSize);
    }

    if( showBoundingBox && boundingBox != nullptr ){
        if(prevCull)
            glDisable(GL_CULL_FACE);

        glLineWidth(2);
        boundingBox->render(shader, false);
        glLineWidth(1);

        if(prevCull)
            glEnable(GL_CULL_FACE);
    }
}

void Model::update(){

}

Model::Model(std::string name, std::vector<Mesh*> meshes, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f)){

    this->boundingBox = nullptr;
    this->name = name;
    this->position = position;

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
