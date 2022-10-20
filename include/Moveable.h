#pragma once

class Moveable{
    private:
        
    public:
        virtual void move(const glm::vec3 movement) = 0;
        virtual void rotate(const glm::vec3 rotation) = 0;
        virtual void scaleUp(const glm::vec3 scale) = 0;
};
