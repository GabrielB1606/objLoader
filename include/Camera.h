#pragma once

enum direction {
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

class Camera{
private:
    
    glm::mat4 ViewMatrix;
    glm::vec3 position, worldUp;
    glm::vec3 front, right, up;
    GLfloat pitch, yaw, roll; // pitch->x, yaw->y, roll->z
    GLfloat movementSpeed, sensitivity;

    void updateCameraVectors();

public:
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp, GLfloat movementSpeed, GLfloat sensitivity);
    virtual ~Camera();

    // getters
    const glm::mat4 getViewMatrix();
    const glm::vec3 getPosition();

    // movement
    void updateCameraAngle(const float& deltaTime, const double& offsetX, const double& offsetY);
    void updatePosition(const float& deltaTime, const int direction);
    void updateInput( const float& deltaTime, const int direction, const double& offsetX, const double& offsetY );
};

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp, GLfloat movementSpeed = 0.5f, GLfloat sensitivity=200.f){
    
    this->ViewMatrix = glm::mat4(1.f);

    this->movementSpeed = movementSpeed;
    this->sensitivity = sensitivity;

    this->worldUp = worldUp;
    this->position = position;
    this->right = glm::vec3(0.f);
    this->up = worldUp;

    this->pitch = -20.f;
    this->yaw = -90.f;
    this->roll = 0.f;

    this->updateCameraVectors();

}

Camera::~Camera(){
}

void Camera::updateCameraVectors(){
    // oof https://learnopengl.com/Getting-started/Camera
    this->front.x = cos( glm::radians( this->yaw ) ) * cos( glm::radians(this->pitch) );
    this->front.y = sin( glm::radians( this->pitch ) );
    this->front.z = sin( glm::radians( this->yaw ) ) * cos( glm::radians(this->pitch) );

    this->front = glm::normalize(this->front);
    this->right = glm::normalize( glm::cross( this->front, this->worldUp ) );
    this->up = glm::normalize( glm::cross( this->right, this->front ) );

}

const glm::vec3 Camera::getPosition(){
    return this->position;
}

const glm::mat4 Camera::getViewMatrix(){
    this->updateCameraVectors();
    this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
    return this->ViewMatrix;
}

void Camera::updateCameraAngle(const float& deltaTime, const double& offsetX, const double& offsetY ){

    this->pitch += static_cast<float>(offsetY) * deltaTime * this->sensitivity;
    this->yaw -= static_cast<float>(offsetX) * deltaTime * this->sensitivity;

    if(this->pitch >= 80.f)
        this->pitch = 80.f;
    else if(this->pitch <= -80.f)
        this->pitch = -80.f;

    if( this->yaw > 360.f || this->yaw < -360.f )
        this->yaw = 0.f;

}

void Camera::updatePosition(const float& deltaTime, const int direction){

    switch (direction){
    
        case FORWARD:
            this->position += this->front * this->movementSpeed * deltaTime;
            break;
        case BACKWARD:
            this->position -= this->front * this->movementSpeed * deltaTime;
            break;
        case LEFT:
            this->position -= this->right * this->movementSpeed * deltaTime;
            break;
        case RIGHT:
            this->position += this->right * this->movementSpeed * deltaTime;
            break;      
        case UPWARD:
            this->position += this->up * this->movementSpeed * deltaTime;
            break;
        case DOWNWARD:
            this->position -= this->up * this->movementSpeed * deltaTime;
            break;
    }

}

void Camera::updateInput(const float& deltaTime, const int direction, const double& offsetX, const double& offsetY ){

}