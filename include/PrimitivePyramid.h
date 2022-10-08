class PrimitivePyramid: public Primitive{
private:
    /* data */
public:
    PrimitivePyramid(/* args */);
    virtual ~PrimitivePyramid();
};

PrimitivePyramid::PrimitivePyramid(/* args */){
    Vertex vertices[] = {
        glm::vec3(0.f, 0.5f, 0.f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(0.f, 0.f, 1.f),
        glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f),
        glm::vec3(0.5f, -0.5f, 0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f),

        glm::vec3(0.f, 0.5f, 0.f),  glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(-1.f, 0.f, 0.f),
        glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
        glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(-1.f, 0.f, 0.f),

        glm::vec3(0.f, 0.5f, 0.f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f),
        
        glm::vec3(0.f, 0.5f, 0.f),  glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f), glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.5f, -0.5f, 0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(1.f, 0.f, 0.f)
    };
    
    unsigned nrOfVertices = sizeof(vertices)/sizeof(Vertex);
    
    unsigned nrOfIndices = 0;

    this->set( vertices, nrOfVertices);
}

PrimitivePyramid::~PrimitivePyramid(){
}
