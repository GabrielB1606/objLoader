#pragma once

// STRUCT VERTEX
struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 textcoord;
    glm::vec3 normal;

    Vertex(){}

    Vertex( glm::vec3 position, glm::vec3 color, glm::vec2 textcoord, glm::vec3 normal ){
        this->position = position;
        this->color = color;
        this->textcoord = textcoord;
        this->normal = normal;
    }
};