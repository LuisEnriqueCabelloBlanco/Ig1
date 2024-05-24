#include "Transform.h"

glm::dmat4 Transform::getTrasformMatrix()
{
    return glm::translate(glm::dmat4(1), position_) *
        glm::rotate(glm::dmat4(1), glm::radians(rotation_.x), glm::dvec3(1, 0, 0)) *
        glm::rotate(glm::dmat4(1), glm::radians(rotation_.y), glm::dvec3(0, 1, 0)) *
        glm::rotate(glm::dmat4(1), glm::radians(rotation_.z), glm::dvec3(0, 0, 1))*
        glm::scale(glm::dmat4(1), scale_);
}
