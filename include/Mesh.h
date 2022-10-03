#pragma once

class Mesh{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 ModelMatrix;

    void initVertexData(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices);
    void initVAO();
    void initModelMatrix();

public:
    Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices);
    ~Mesh();
    void update();
    void render(Shader* shader);
    void updateUniforms(Shader* shader);
};

void Mesh::updateUniforms(Shader* shader){
    shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
}

void Mesh::render(Shader* shader){
    // update uniforms
    this->updateUniforms(shader);

    // bind VAO
    glBindVertexArray(this->VAO);

    // render
    shader->use();

    if(this->indices.empty())
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    else
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

    shader->stopUsing();

}

Mesh::Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices){

    initVertexData(vertexArray, nrOfVertices, indexArray, nrOfIndices);
    initVAO();
    initModelMatrix();

}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Mesh::initModelMatrix(){

    this->position = glm::vec3(0.f);
    this->rotation = glm::vec3(0.f);
    this->scale = glm::vec3(1.f);

    this->ModelMatrix = glm::mat4(1.f);

    this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians( this->rotation.x ), glm::vec3(1.f, 0.f, 0.f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians( this->rotation.y ), glm::vec3(0.f, 1.f, 0.f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians( this->rotation.z ), glm::vec3(0.f, 0.f, 1.f));
    this->ModelMatrix = glm::scale(this->ModelMatrix, glm::vec3( this->scale ));

}

void Mesh::initVAO(){
    // VAO
    glCreateVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // VBO
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); // could be dynamic draw

    // EBO
    glGenBuffers(1, &this->EBO);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

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

void Mesh::initVertexData(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices){
    
    for (size_t i = 0; i < nrOfVertices; i++){
        this->vertices.push_back( vertexArray[i] );
    }
    
    for (size_t i = 0; i < nrOfIndices; i++){
        this->indices.push_back( indexArray[i] );
    }
    
}