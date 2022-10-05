#pragma once

class Primitive{
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
public:
    Primitive();
    ~Primitive();
    void set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices);
    inline Vertex* getVertices();
    inline GLuint* getIndices();
    inline const unsigned getNrOfVertices();
    inline const unsigned getNrOfIndices();
};

inline const unsigned Primitive::getNrOfVertices(){ return this->vertices.size(); }

inline const unsigned Primitive::getNrOfIndices(){ return this->indices.size(); }

inline GLuint* Primitive::getIndices(){ return this->indices.data(); }

inline Vertex* Primitive::getVertices(){ return this->vertices.data(); }

Primitive::Primitive(/* args */){}

void Primitive::set(const Vertex* vertices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices){
    for (size_t i = 0; i < nrOfVertices; i++)
        this->vertices.push_back(vertices[i]);
    
    for (size_t i = 0; i < nrOfIndices; i++)
        this->indices.push_back(indices[i]);
}

Primitive::~Primitive(){
}
