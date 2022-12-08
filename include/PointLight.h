#pragma once

class PointLight : public Light, public Moveable{
protected:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    /* data */
public:
    PointLight(glm::vec3 position, float intensity, glm::vec3 color, float constant, float linear, float quadratic);
    virtual ~PointLight();

    void sendToShader( Shader& program );
    void setPosition( glm::vec3 position );

    void move(const glm::vec3 movement);
    void rotate(const glm::vec3 rotation){}
    void scaleUp(const glm::vec3 scale){};

    float* getConstantReference(){ return &this->constant; }
    float* getLinearReference(){ return &this->linear; }
    float* getQuadraticReference(){ return &this->quadratic; }
};

void PointLight::move(const glm::vec3 movement){
    position += movement;
}

void PointLight::setPosition(glm::vec3 position){
    this->position = position;
}

PointLight::PointLight(glm::vec3 position, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f), float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f): Light(intensity, color){
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

PointLight::~PointLight(){
}

void PointLight::sendToShader(Shader& program){
    program.setVec3f(this->position, "pointLight.position");
    program.set1f(this->intensity, "pointLight.intensity");
    program.setVec3f(this->color, "pointLight.color");
    program.set1f(this->constant, "pointLight.constant");
    program.set1f(this->linear, "pointLight.linear");
    program.set1f(this->quadratic, "pointLight.quadratic");
}
