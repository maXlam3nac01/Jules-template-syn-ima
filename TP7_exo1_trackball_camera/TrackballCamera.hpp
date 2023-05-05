#include "glm/gtx/transform.hpp"

#pragma once

class TrackballCamera {
    float m_Distance; // distance par rapport au centre
    float m_AngleX;   // angle effectué par la caméra autour de l'axe X de la scène (rotation vers le haut ou vers le bas)
    float m_AngleY;   // angle effectué par la caméra autour de l'axe Y de la scène (rotation vers la gauche ou vers la droite)

public:
    TrackballCamera(float m_Distance, float m_AngleX, float m_AngleY)
        : m_Distance(m_Distance = 5), m_AngleX(m_AngleX = 0), m_AngleY(m_AngleY = 0) {}

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};
