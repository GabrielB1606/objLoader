#pragma once

class Mesh{
private:
    
    Vertex* vertexArray;
    GLuint* indexArray;

    unsigned nrOfVertices;
    unsigned nrOfIndices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 origin;

    glm::mat4 ModelMatrix;

    void initVAO();

public:
    
    Mesh(const Mesh &obj);
    Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, glm::vec3 origin, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    Mesh(Primitive &primitive, glm::vec3 origin, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    
    virtual ~Mesh();
    
    void update();
    void render(Shader* shader);
    
    void updateUniforms(Shader* shader);
    void updateModelMatrix();
    
    void move(const glm::vec3 movement);
    void rotate(const glm::vec3 rotation);
    void scaleUp(const glm::vec3 scale);

    void setPosition(const glm::vec3 position);
    void setRotation(const glm::vec3 rotation);
    void setScale(const glm::vec3 scale);
    void setOrigin(const glm::vec3 origin);
};

Mesh::Mesh(const Mesh &obj){

    this->nrOfIndices = obj.nrOfIndices;
    this->nrOfVertices = obj.nrOfVertices;

    this->vertexArray = new Vertex[ this->nrOfVertices ];
    for( size_t i = 0; i<this->nrOfVertices; i++ )
        this->vertexArray[i] = obj.vertexArray[i];

    this->indexArray = new GLuint[ this->nrOfIndices ];
    for( size_t i = 0; i<obj.nrOfIndices; i++ )
        this->indexArray[i] = obj.indexArray[i];

    initVAO();
    
    this->position = obj.position;
    this->rotation = obj.rotation;
    this->scale = obj.scale;
    this->origin = obj.origin;

}

void Mesh::scaleUp(const glm::vec3 scale){
    this->scale += scale;
}

void Mesh::rotate( const glm::vec3 rotation ){
    this->rotation += rotation;
}

void Mesh::move(const glm::vec3 movement){
    this->position += movement;
}

void Mesh::setRotation( const glm::vec3 rotation ){
    this->rotation = rotation;
}

void Mesh::setScale( const glm::vec3 scale ){
    this->scale = scale;
}

void Mesh::setPosition( const glm::vec3 position ){
    this->position = position;
}

void Mesh::updateModelMatrix(){
    this->ModelMatrix = glm::mat4(1.f);

    this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians( this->rotation.x ), glm::vec3(1.f, 0.f, 0.f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians( this->rotation.y ), glm::vec3(0.f, 1.f, 0.f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians( this->rotation.z ), glm::vec3(0.f, 0.f, 1.f));
    this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
    this->ModelMatrix = glm::scale(this->ModelMatrix, glm::vec3( this->scale ));
}

void Mesh::updateUniforms(Shader* shader){
    shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

void Mesh::render(Shader* shader){
    
    // Model Matrix update
    this->updateModelMatrix();

    // update uniforms
    this->updateUniforms(shader);

    // bind VAO
    glBindVertexArray(this->VAO);

    // render
    shader->use();

    if(this->nrOfIndices == 0)
        glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
    else
        glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);

    // cleanup
    shader->stopUsing();
    glBindVertexArray(0);

}

void Mesh::setOrigin( const glm::vec3 origin ){
    this->origin = origin;
}

Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, glm::vec3 origin = glm::vec3(0.f), glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){

    this->nrOfIndices = nrOfIndices;
    this->nrOfVertices = nrOfVertices;

    this->vertexArray = new Vertex[ this->nrOfVertices ];
    for( size_t i = 0; i<nrOfVertices; i++ )
        this->vertexArray[i] = vertexArray[i];

    this->indexArray = new GLuint[ this->nrOfIndices ];
    for( size_t i = 0; i<nrOfIndices; i++ )
        this->indexArray[i] = indexArray[i];

    initVAO();
    
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->origin = origin;

}

Mesh::Mesh(Primitive &primitive, glm::vec3 origin = glm::vec3(0.f), glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){

    this->nrOfIndices = primitive.getNrOfIndices();
    this->nrOfVertices = primitive.getNrOfVertices();

    this->vertexArray = new Vertex[ this->nrOfVertices ];
    for( size_t i = 0; i<nrOfVertices; i++ )
        this->vertexArray[i] = primitive.getVertices()[i];

    this->indexArray = new GLuint[ this->nrOfIndices ];
    for( size_t i = 0; i<nrOfIndices; i++ )
        this->indexArray[i] = primitive.getIndices()[i];

    initVAO();
    
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->origin = origin;

}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);

    delete this->vertexArray;
    delete this->indexArray;

}

void Mesh::initVAO(){
    
    // VAO
    glCreateVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // VBO
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), vertexArray, GL_STATIC_DRAW); // could be dynamic draw

    // EBO
    if( this->nrOfIndices > 0 ){
        glGenBuffers(1, &this->EBO);
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices *sizeof(GLuint), indexArray, GL_STATIC_DRAW);
    }
    
    // VERTEXATTRIBUTEPOINTERS AND ENABLE (INPUT ASSEMBLY)
        // POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

        // COLOR
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

        // TEXTCOORD
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textcoord));
    glEnableVertexAttribArray(2);

        // NORMAL
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    // BIND VAO 0
    glBindVertexArray(0);
}