#pragma once

class DirectionalLight : public Light{
protected:
    glm::vec3 direction;
    /* data */
public:
    DirectionalLight(glm::vec3 direction, float intensity, glm::vec3 color);
    virtual ~DirectionalLight();

    void sendToShader( Shader& program, GLuint i );

    glm::vec3* getDirectionReference(){ return &this->direction; }
};

DirectionalLight::DirectionalLight(glm::vec3 direction, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f)): Light(intensity, color){
    this->direction = direction;
}

DirectionalLight::~DirectionalLight(){
}

void DirectionalLight::sendToShader(Shader& program, GLuint i = 0){
    program.set1i(DIR_LIGHT, std::string( "lights["+ std::to_string(i) +"].type" ).c_str());

    program.setVec3f(this->direction, std::string( "lights["+ std::to_string(i) +"].position" ).c_str()  );
    program.set1f(this->intensity, std::string( "lights["+ std::to_string(i) +"].intensity" ).c_str());
    program.setVec3f(this->color, std::string( "lights["+ std::to_string(i) +"].color" ).c_str());
}
