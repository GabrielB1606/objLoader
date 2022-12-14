#pragma once

// STRUCT VERTEX
struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 textcoord;
    glm::vec3 normal;

    Vertex(){}

    Vertex( glm::vec3 position, glm::vec3 color = glm::vec3(0.7f), glm::vec2 textcoord = glm::vec2(0.f), glm::vec3 normal = glm::vec3(0.f) ){
        this->position = position;
        this->color = color;
        this->textcoord = textcoord;
        this->normal = normal;
    }
};