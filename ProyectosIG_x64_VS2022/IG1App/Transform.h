#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Transform
{
	glm::dvec3 position_;
	glm::dvec3 rotation_;
	glm::dvec3 scale_;

public:
	Transform() {
		position_ = { 0,0,0 };
		rotation_ = { 0,0,0 };
		scale_ = { 1,1,1 };
	}
	void setPosition(glm::dvec3 pos) { position_ = pos; };
	void setRotation(glm::dvec3 rot) { rotation_ = rot; };
	void setScale(glm::dvec3 sc) { scale_ = sc; };

	glm::dvec3& const getPosition() {return position_;};
	glm::dvec3& const getRotation() { return rotation_; };
	glm::dvec3& const getScale() { return scale_; };
	glm::dmat4 getTrasformMatrix();
};

