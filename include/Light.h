#pragma once

class Light{
protected:
    float intensity;
    glm::vec3 color;
    GLuint type;
public:
    Light(float intensity, glm::vec3 color, GLuint type );
    virtual ~Light();

    void sendToShader( Shader &program ){}

    GLuint getType(){ return this->type; }

    glm::vec3* getColorReference(){ return &this->color; }
    float* getIntensityReference(){ return &this->intensity; }

};

Light::Light(float intensity, glm::vec3 color, GLuint type = POINT_LIGHT){
    this->intensity = intensity;
    this->color = color;
    this->type = type;
}



Light::~Light(){
}
