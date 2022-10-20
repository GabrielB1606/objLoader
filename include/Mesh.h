#pragma once

class Mesh : public Moveable{
private:
    
    std::string name;

    GLenum renderType;

    Vertex* vertexArray = nullptr;
    unsigned int* indexArray = nullptr;

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
    Material* material;


public:

    void initVAO();
    Mesh(std::string name, std::vector<glm::vec3> &positionVertex, std::vector<glm::vec2> &textcoordVertex, std::vector<glm::vec3> &normalVertex, std::vector<GLuint> &positionIndex, std::vector<GLuint> &textcoordIndex, std::vector<GLuint> normalIndex, Material* material, GLenum renderType);
    
    Mesh(const Mesh &obj);
    Mesh(Material* material, Vertex* vertexArray, const unsigned& nrOfVertices, GLenum renderType, GLuint* indexArray, const unsigned& nrOfIndices, glm::vec3 origin, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    Mesh(Primitive &primitive, glm::vec3 origin, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    

    virtual ~Mesh();
    
    void update();
    void render(Shader* shader, bool showFill, bool showEdges, bool showVertices, float vertexSize);
    
    void updateUniforms(Shader* shader);
    void updateModelMatrix();
    
    void move(const glm::vec3 movement);
    void rotate(const glm::vec3 rotation);
    void scaleUp(const glm::vec3 scale);

    void normalize(const float factor);

    void setPosition(const glm::vec3 position);
    void setRotation(const glm::vec3 rotation);
    void setScale(const glm::vec3 scale);
    void setOrigin(const glm::vec3 origin);

    std::string getName();
    Material* getMaterialReference();
};

Material* Mesh::getMaterialReference(){
    return this->material;
}

std::string Mesh::getName(){
    return this->name;
}

void Mesh::normalize(const float factor){

    // glDeleteVertexArrays(1, &this->VAO);
    // glDeleteBuffers(1, &this->VBO);
    // glDeleteBuffers(1, &this->EBO);

    for(size_t i = 0; i < nrOfVertices; i++){
        vertexArray[i].position.x /= factor;
        vertexArray[i].position.y /= factor;
        vertexArray[i].position.z /= factor;
    }

    initVAO();

}



Mesh::Mesh(std::string name, std::vector<glm::vec3> &positionVertex, std::vector<glm::vec2> &textcoordVertex, std::vector<glm::vec3> &normalVertex, std::vector<GLuint> &positionIndex, std::vector<GLuint> &textcoordIndex, std::vector<GLuint> normalIndex, Material* material, GLenum renderType = GL_TRIANGLES){

    this->name = name;
    this->renderType = renderType;
    this->material = material;

    std::unordered_map< std::pair<GLuint, GLuint> , size_t, HashPair> vertexMap;
    std::vector<Vertex> vertexArray;
    std::vector<GLuint> indexVector;

    std::pair<GLuint, GLuint> key;


    for( size_t i = 0; i<positionIndex.size(); i++ ){

        key = std::pair<GLuint, GLuint>( positionIndex[i], normalIndex[i] );

        if( vertexMap.find(key) == vertexMap.end() ){

            vertexMap[key] = vertexArray.size();
            vertexArray.push_back( 
                textcoordVertex.size() >0?
                Vertex( 
                    positionVertex[ positionIndex[i]-1 ],
                    glm::vec3(0.7f),
                    textcoordVertex[ textcoordIndex[i]-1 ],
                    normalVertex[ normalIndex[i]-1 ]
                ):
                Vertex( 
                    positionVertex[ positionIndex[i]-1 ],
                    glm::vec3(0.7f),
                    glm::vec2(0.f),
                    normalVertex[ normalIndex[i]-1 ]
                )
            );

        }

        indexVector.push_back(vertexMap[key]);
    
    }

    this->nrOfIndices = indexVector.size();
    this->indexArray = new GLuint[ this->nrOfIndices ];

    for(size_t i = 0; i<this->nrOfIndices; i++)
        this->indexArray[i] = indexVector[i];

    
    this->nrOfVertices = vertexArray.size();
    this->vertexArray = new Vertex[ this->nrOfVertices ];
    for(size_t i = 0; i<this->nrOfVertices; i++){
        this->vertexArray[i].color = vertexArray[i].color;
        this->vertexArray[i].normal = vertexArray[i].normal;
        this->vertexArray[i].position = vertexArray[i].position;
        this->vertexArray[i].textcoord = vertexArray[i].textcoord;
    }

    // initVAO();
    this->position = glm::vec3(0.f);
    this->rotation = glm::vec3(0.f);
    this->scale = glm::vec3(1.f);
    this->origin = glm::vec3(0.f);

}

Mesh::Mesh(const Mesh &obj){

    this->name = obj.name;
    this->material = obj.material;
    this->renderType = obj.renderType;
    this->nrOfIndices = obj.nrOfIndices;
    this->nrOfVertices = obj.nrOfVertices;

    this->vertexArray = new Vertex[ this->nrOfVertices ];
    for( size_t i = 0; i<this->nrOfVertices; i++ )
        this->vertexArray[i] = obj.vertexArray[i];

    if( obj.nrOfIndices > 0 ){
        this->indexArray = new GLuint[ this->nrOfIndices ];
        for( size_t i = 0; i<obj.nrOfIndices; i++ )
            this->indexArray[i] = obj.indexArray[i];
    }else
        this->indexArray = nullptr;
 
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

void Mesh::render(Shader* shader, bool showFill = true, bool showEdges = true, bool showVertices = false, float vertexSize = 0.f){
    
    // Model Matrix update
    this->updateModelMatrix();

    // update uniforms
    this->updateUniforms(shader);

    // bind VAO
    glBindVertexArray(this->VAO);

    // render

    if(showFill){
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonOffset(4.0, 1.0);
        material->sendToShader(*shader, GL_FILL);

        shader->use();
        if(this->nrOfIndices == 0)
            glDrawArrays(this->renderType, 0, this->nrOfVertices);
        else
            glDrawElements(this->renderType, this->nrOfIndices, GL_UNSIGNED_INT, 0);
                
    }

    if( showEdges ){

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonOffset(2.0, 1.0);
        material->sendToShader(*shader, GL_LINE);

        shader->use();
        if(this->nrOfIndices == 0)
            glDrawArrays(this->renderType, 0, this->nrOfVertices);
        else
            glDrawElements(this->renderType, this->nrOfIndices, GL_UNSIGNED_INT, 0);
        
    }

    if( showVertices ){

        glPointSize(vertexSize);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        material->sendToShader(*shader, GL_POINT);

        shader->use();
        if(this->nrOfIndices == 0)
            glDrawArrays(this->renderType, 0, this->nrOfVertices);
        else
            glDrawElements(this->renderType, this->nrOfIndices, GL_UNSIGNED_INT, 0);

        glPointSize(1.f);
        
    }


    // cleanup
    shader->stopUsing();
    glBindVertexArray(0);

}

void Mesh::setOrigin( const glm::vec3 origin ){
    this->origin = origin;
}

Mesh::Mesh(Material* material, Vertex* vertexArray, const unsigned& nrOfVertices, GLenum renderType = GL_TRIANGLES, GLuint* indexArray = nullptr, const unsigned& nrOfIndices = 0, glm::vec3 origin = glm::vec3(0.f), glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){
    this->material = material;
    this->renderType = renderType;
    this->nrOfIndices = nrOfIndices;
    this->nrOfVertices = nrOfVertices;

    this->vertexArray = new Vertex[ this->nrOfVertices ];
    for( size_t i = 0; i<nrOfVertices; i++ )
        this->vertexArray[i] = vertexArray[i];

    if( nrOfIndices > 0 ){
        this->indexArray = new GLuint[ this->nrOfIndices ];
        for( size_t i = 0; i<nrOfIndices; i++ )
            this->indexArray[i] = indexArray[i];
    }else
        this->indexArray = nullptr;

    initVAO();
    
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->origin = origin;

}

Mesh::Mesh(Primitive &primitive, glm::vec3 origin = glm::vec3(0.f), glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)){

    this->renderType = GL_TRIANGLES;

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