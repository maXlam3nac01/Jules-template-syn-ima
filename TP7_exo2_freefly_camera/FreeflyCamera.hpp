#include "glm/ext/scalar_constants.hpp"
#include "glm/gtx/transform.hpp"

#pragma once

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    void computeDirectionVectors();

public:
    FreeflyCamera()
        : m_Position(glm::vec3(0.f)), m_Phi(glm::pi<float>()), m_Theta(0.f)
    {
        computeDirectionVectors();
    }
    void moveLeft(float t);
    void moveFront(float t);

    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};
