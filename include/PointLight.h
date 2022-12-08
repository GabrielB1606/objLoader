#pragma once

class PointLight : public Light{
protected:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    /* data */
public:
    PointLight(glm::vec3 position, float intensity, glm::vec3 color, float constant, float linear, float quadratic);
    virtual ~PointLight();

    void sendToShader( Shader& program, GLuint i );
    void setPosition( glm::vec3 position );

    float* getConstantReference(){ return &this->constant; }
    float* getLinearReference(){ return &this->linear; }
    float* getQuadraticReference(){ return &this->quadratic; }

    glm::vec3* getPositionReference(){ return &this->position; }
};

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

void PointLight::sendToShader(Shader& program, GLuint i = 0){
    program.setVec3f(this->position, std::string( "lights["+ std::to_string(i) +"].position" ).c_str()  );
    program.set1f(this->intensity, std::string( "lights["+ std::to_string(i) +"].intensity" ).c_str());
    program.setVec3f(this->color, std::string( "lights["+ std::to_string(i) +"].color" ).c_str());
    program.set1f(this->constant, std::string( "lights["+ std::to_string(i) +"].constant" ).c_str());
    program.set1f(this->linear, std::string( "lights["+ std::to_string(i) +"].linear" ).c_str());
    program.set1f(this->quadratic, std::string( "lights["+ std::to_string(i) +"].quadratic" ).c_str());
}
