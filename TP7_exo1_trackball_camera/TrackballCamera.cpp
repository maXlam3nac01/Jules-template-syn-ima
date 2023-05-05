#include "TrackballCamera.hpp"

void TrackballCamera::moveFront(float delta)
{
    // if (delta > 0)
    // {
    //     m_Distance += 2;
    // }
    // if (delta < 0)
    // {
    //     m_Distance -= 2;
    // }
    m_Distance += delta;
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_AngleY += glm::radians(degrees);
}

void TrackballCamera::rotateUp(float degrees)
{
    m_AngleX += glm::radians(degrees);
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::vec3 position(0.0f, 0.0f, m_Distance);
    glm::mat4 view(1.0f);

    view = glm::rotate(view, glm::radians(m_AngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(m_AngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, -position);

    return view;
}
