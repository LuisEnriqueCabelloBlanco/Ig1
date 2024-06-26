#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <iostream>
using namespace glm;

Camera::Camera(Viewport* vp)
  : mViewMat(1.0)
  , mProjMat(1.0)
  , xRight(vp->width() / 2.0)
  , xLeft(-xRight)
  , yTop(vp->height() / 2.0)
  , yBot(-yTop)
  , mViewPort(vp)
{
	setPM();
}

void Camera::setCenital()
{
	mEye = mLook + dvec3(0, mRadio, 0);

	mUp = dvec3(0, 0, -1);
	setVM();
}

void
Camera::uploadVM() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(mViewMat)); // transfers view matrix to the GPU
}

void Camera::setAxes()
{
	mRight = glm::row(mViewMat,0);
	mUpward = glm::row(mViewMat,1);
	mFront = glm::row(mViewMat,2);
}

void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = dvec3(0, 0, mRadio);
	mLook = dvec3(0, 0, 0);
	mUp = dvec3(0, 1, 0);
	setVM();
	//descomentar para el ejercicio 47
	//mViewMat = glm::translate(mViewMat, dvec3(0,-200,0));
	//mLook = dvec3(200, 0, 0);
}

void
Camera::set3D()
{
	mEye = dvec3(mRadio, mRadio, mRadio);
	mLook = dvec3(0, 10, 0);
	mUp = dvec3(0, 1, 0);
	setVM();
	//Descomentar para el ejercicio 47
	//mViewMat = glm::translate(mViewMat, dvec3(0, 200, 0));
	//mLook = dvec3(200, 0, 0);
}

void
Camera::pitch(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
	setAxes();
}

void
Camera::yaw(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
	// glm::rotate returns mViewMat * rotationMatrix
	setAxes();
}

void
Camera::roll(GLdouble a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
	// glm::rotate returns mViewMat * rotationMatrix
	setAxes();
}

void Camera::pitchReal(GLdouble cs)
{
	mProjMat = rotate(mProjMat, glm::radians(cs), glm::dvec3(1, 0, 0));
	//setAxes();
}

void Camera::yawReal(GLdouble cs)
{
	mProjMat = rotate(mProjMat, glm::radians(cs), glm::dvec3(0, 1, 0));
}

void Camera::rollReal(GLdouble cs)
{
	mProjMat = rotate(mProjMat, glm::radians(cs), glm::dvec3(0, 0, 1));
}

void Camera::moveLR(GLdouble cs) //ummm ackshually lo que esta comentado es la logica que hay que seguir no? pero se nos reinicia la posicion de la camara en los eventos del raton
{
	mViewMat = glm::translate(mViewMat, mRight * cs);
	setAxes();
	//mEye += mRight * cs;
	//mLook += mRight * cs;
	//setVM();
}

void Camera::moveFB(GLdouble cs)
{
	//mEye += mFront * cs;
	//mLook += mFront * cs;
	//setVM();
	mViewMat = glm::translate(mViewMat, mFront * cs);
	setAxes();
}

void Camera::moveUD(GLdouble cs) //mirar comentario de moveLR
{
	//mEye += mUpward * cs;
	//mLook += mUpward * cs;
	//setVM();
	mViewMat = glm::translate(mViewMat, mUpward * cs);
	setAxes();
}

void Camera::changePrj()
{
	bOrto = !bOrto;
	setPM();
}

void Camera::update()
{
	//apartado 47
	/*mViewMat = glm::rotate(mViewMat, glm::radians(-5.0), glm::dvec3(0.0, 0.0, 1.0));
	glm::dmat4 circleRot = glm::rotate(glm::dmat4(1.0), glm::radians(2.5), glm::dvec3(0.0, 0.0, 1.0));
	mViewMat = circleRot * mViewMat;*/
	//orbit(5, 200);
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{

	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;

	if (!bOrto) {
		mEye += mFront * s;
	}
	setPM();
}

void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
		                 xRight * mScaleFact,
		                 yBot * mScaleFact,
		                 yTop * mScaleFact,
		                 mNearVal,
		                 mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else {
		mProjMat = frustum(xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			yTop,
			mFarVal);
	}
}

void Camera::orbit(GLdouble incAng, GLdouble inc)
{
	mAng += incAng;
	
	//auto a  = mLook* dvec3(1, 0, 0);
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	//no se si realmente se deberia incrementar la altura constantemente ya que eso no describe un orbita
	mEye.y = inc;
	setVM();
}

void
Camera::uploadPM() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(mProjMat)); // transfers projection matrix to the GPU
	glMatrixMode(GL_MODELVIEW);
}
