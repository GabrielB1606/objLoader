#pragma once

class Light{
protected:
    float intensity;
    glm::vec3 color;
public:
    Light(float intensity, glm::vec3 color);
    virtual ~Light();
};

Light::Light(float intensity, glm::vec3 color){
    this->intensity = intensity;
    this->color = color;
}

Light::~Light(){
}
