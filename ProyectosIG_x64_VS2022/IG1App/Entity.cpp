#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void Abs_Entity::update()
{
}

void
Abs_Entity::upload(dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat)); // transfers modelView matrix to the GPU
}

EjesRGB::EjesRGB(GLdouble l)
  : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}

EjesRGB::~EjesRGB()
{
	delete mMesh;
	mMesh = nullptr;
};

void
EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r) : Abs_Entity()
{
	mMesh = Mesh::generateRegularPolygon(num, r);
}

RegularPolygon::~RegularPolygon() {
	delete mMesh;
	mMesh = nullptr;
}

void RegularPolygon::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glColor4d(mColor.r, mColor.g, mColor.b, mColor.a);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glColor4d(0.0, 0.0, 0.0, 1.0);
	}
}

RGBTriangle::RGBTriangle()
{
	std::vector<glm::dvec4> colors;
	colors.reserve(3);
	colors.emplace_back(1.0, 0.0, 0.0, 1.0);
	colors.emplace_back(0.0, 1.0, 0.0, 1.0);
	colors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mMesh = Mesh::generateRegularPolygon(3, 100,colors);
}

RGBTriangle::~RGBTriangle()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBTriangle::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		//glEnable(GL_CULL_FACE);
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glLineWidth(2);
		//esta instruccion ajusta que cara se pinta y con que modo
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_LINE);
		mMesh->render();
		//mMesh->render();
		glFrontFace(GL_CCW);
		glLineWidth(1);
		glColor4d(0.0, 0.0, 0.0, 1.0);
		//glDisable(GL_CULL_FACE);
	}
}

void RGBTriangle::update()
{
	mModelMat = glm::rotate(mModelMat, glm::radians(-10.0), glm::dvec3(0.0, 0.0, 1.0));
	glm::dmat4 circleRot = glm::rotate(glm::dmat4(1.0), glm::radians(5.0), glm::dvec3(0.0, 0.0, 1.0));
	mModelMat = circleRot * mModelMat;
}


RGBRectangle::RGBRectangle(GLdouble w,GLdouble h) {
	mMesh = Mesh::generateRGBRectangle(w,h);
}

RGBRectangle::~RGBRectangle()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBRectangle::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glLineWidth(2);
		//esta instruccion ajusta que cara se pinta y con que modo
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glPolygonMode(GL_BACK, GL_LINE);
		mMesh->render();
		glPolygonMode(GL_FRONT, GL_LINE);
		glFrontFace(GL_CCW);
		glLineWidth(1);
		glColor4d(0.0, 0.0, 0.0, 1.0);
	}
}

Cube::Cube(GLdouble length)
{
	mMesh = Mesh::generateCube(length);
}

Cube::~Cube()
{
	delete mMesh;
	mMesh = nullptr;
}

void Cube::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glLineWidth(2);
		//esta instruccion ajusta que cara se pinta y con que modo
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_POINT);
		mMesh->render();
		glFrontFace(GL_CCW);
		glLineWidth(1);
		glColor4d(0.0, 0.0, 0.0, 1.0);
	}
}

RGBCube::RGBCube(GLdouble length)
{
	mMesh = Mesh::generateRGBCubeTriangles(length);
	
}

RGBCube::~RGBCube()
{
	delete mMesh;
	mMesh = nullptr;
}

void RGBCube::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glLineWidth(2);
		//esta instruccion ajusta que cara se pinta y con que modo
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_POINT);
		mMesh->render();
		glFrontFace(GL_CCW);
		glLineWidth(1);
		glColor4d(0.0, 0.0, 0.0, 1.0);
	}
}

void RGBCube::update()
{
	dvec3 direcionRot;
	
	switch (currentState) {
	case rotX:
		direcionRot = dvec3(1.0, 0.0, 0.0);
		break;
	case rotY:
		direcionRot = dvec3(0.0, 1.0, 0.0);
		break;
	case rotZ:
		direcionRot = dvec3(0.0, 0.0, 1.0);
		break;
	default:
		break;
	}


	if (rotatedAngles > 180) {
		rotatedAngles = 0;
		switch (currentState) {
		case rotX:
			currentState = rotZ;
			break;
		case rotY:
			currentState = rotX;
			break;
		case rotZ:
			currentState = rotY;
			break;
		default:
			break;
		}
	}

	glm::dmat4 rotMat = glm::rotate(dmat4(1.0), glm::radians(rotSpeed), direcionRot);
	rotatedAngles += rotSpeed;
	mModelMat = rotMat* mModelMat;
}

Ground::Ground(GLdouble w, GLdouble h)
{
	//somos bobos y no habiamos cambiado la malla
	mMesh = Mesh::generateRectangleTexCor(w, h,4,4);
	mModelMat = glm::rotate(dmat4(1.0), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));

}

Ground::~Ground()
{
	delete mMesh;
	mMesh = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
}

void Ground::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		mTexture->bind(GL_MODULATE);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		mTexture->unbind();
	}
}

BoxOutline::BoxOutline(GLdouble length)
{
	mMesh = Mesh::generateBoxOutlineTexCor(length);
}

BoxOutline::~BoxOutline()
{
	delete mMesh;
	mMesh = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
	if (mBackTexture != nullptr) {
		delete mBackTexture;
		mBackTexture = nullptr;
	}
}

void BoxOutline::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glEnable(GL_CULL_FACE);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_BACK, GL_FILL);
		mTexture->bind(GL_MODULATE);
		glCullFace(GL_BACK);
		mMesh->render();
		mTexture->unbind();

		mBackTexture->bind(GL_MODULATE);
		glCullFace(GL_FRONT);
		mMesh->render();
		mBackTexture->unbind();


		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		glDisable(GL_CULL_FACE);
	}
}

Star3D::Star3D(GLdouble re, GLuint np, GLdouble h)
{
	mMesh = Mesh::generateStar3D(re,np,h);
}

Star3D::~Star3D()
{
	delete mMesh;
	mMesh = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
}

void Star3D::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		mTexture->bind(GL_MODULATE);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		mMesh->render();

		dmat4 aux = aMat * glm::rotate(dmat4(1), glm::radians(180.0), dvec3(1.0, 0.0, 0.0));
		upload(aux);
		mMesh->render();

		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		mTexture->unbind();
	}
}

void Star3D::update()
{
	mModelMat = glm::rotate(mModelMat, glm::radians(1.0), dvec3(0.0, 0.0, 1.0));
	dmat4 aux = glm::rotate(dmat4(1), glm::radians(1.0), dvec3(0.0, 1.0, 0.0));
	mModelMat =  mModelMat * aux;
}

Box::Box(GLdouble length)
{
	rotationSing = -1;
	rotationSpeed = 1.0;
	rotatedAngle = 0;
	mLength = length;
	mMesh = Mesh::generateBoxOutlineTexCor(length);
	upperCase = Mesh::generateRectangleTexCor(length, length);
	mModelMatUpperCase = glm::translate(dmat4(1.0),dvec3(0.0, length / 2, 0.0));
	mModelMatUpperCase = glm::rotate(dmat4(mModelMatUpperCase), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));
	underCase = Mesh::generateRectangleTexCor(length, length);
	mModelMatUnderCase = glm::translate(dmat4(1.0),dvec3(0.0, -length / 2, 0.0));
	mModelMatUnderCase = glm::rotate(mModelMatUnderCase, glm::radians(-90.0), glm::dvec3(1.0, 0.0, 0.0));
}

Box::~Box()
{
	delete mMesh;
	delete underCase;
	delete upperCase;
	mMesh = nullptr;
	upperCase = nullptr;
	underCase = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
	if (mBackTexture != nullptr) {
		delete mBackTexture;
		mBackTexture = nullptr;
	}
}

void Box::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		glEnable(GL_CULL_FACE);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->bind(GL_MODULATE);
		glCullFace(GL_BACK);

		mMesh->render();
		upload(aMat*mModelMatUpperCase);
		upperCase->render();
		upload(aMat*mModelMatUnderCase);
		underCase->render();

		mTexture->unbind();

		mBackTexture->bind(GL_MODULATE);
		glCullFace(GL_FRONT);
		upload(aMat);
		mMesh->render();
		upload(aMat * mModelMatUpperCase);
		upperCase->render();
		upload(aMat * mModelMatUnderCase);
		underCase->render();

		mBackTexture->unbind();


		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		glDisable(GL_CULL_FACE);
	}
}

void Box::update()
{
	dmat4 rotation = glm::rotate(dmat4(1), glm::radians(rotationSpeed*rotationSing), glm::dvec3(0.0, 1.0, 0.0));
	dmat4 traslation1 = glm::translate(dmat4(1), dvec3(-mLength/2, 0.0, 0.0));
	dmat4 traslation2 = glm::translate(dmat4(1), dvec3(mLength/2, 0.0, 0.0));
	mModelMatUpperCase = mModelMatUpperCase * traslation2 * rotation * traslation1;

	rotatedAngle += rotationSpeed;
	if (rotatedAngle > 180) {
		if (rotationSing == -1)
			rotationSing = 1;
		else 
			rotationSing = -1;
		rotatedAngle = 0;
	}
}

GlassParapet::GlassParapet(int size)
{
	mMesh = Mesh::generateBoxOutlineTexCor(size);
}

GlassParapet::~GlassParapet()
{
	delete mMesh;
	mMesh = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
}

void GlassParapet::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		mTexture->bind(GL_MODULATE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		glDisable(GL_BLEND);
		mTexture->unbind();
	}
}

Grass::Grass()
{
	mMesh = Mesh::generateRectangleTexCor(100, 100);
}

Grass::~Grass()
{
	delete mMesh;
	mMesh = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
}

void Grass::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		mTexture->bind(GL_REPLACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();
		upload(glm::rotate(aMat, glm::radians(60.0), dvec3(0.0, 1.0, 0.0)));
		mMesh->render();
		upload(glm::rotate(aMat, glm::radians(120.0), dvec3(0.0, 1.0, 0.0)));
		mMesh->render();
		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		mTexture->unbind();
	}
}

Photo::Photo(GLsizei width, GLsizei height, GLuint buffer) : _width(width), _height(height), _buffer(buffer)
{
	mMesh = Mesh::generateRectangleTexCor(_width, _height, 1, 1);
	mModelMat = glm::translate(dmat4(1.0), glm::dvec3(0, 1, 0)); //para que no este al nivel del suelo y se pueda ver
	mModelMat = glm::rotate(modelMat(), glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0)); 
}

Photo::~Photo()
{
	delete mMesh;
	mMesh = nullptr;
	if (mTexture != nullptr) {
		delete mTexture;
		mTexture = nullptr;
	}
}

void Photo::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		upload(aMat); //mandar mesh a gpu
		mTexture->bind(GL_REPLACE);
		glLineWidth(2);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT, GL_POINTS);
		mMesh->render();
		glColor4d(0.0, 0.0, 0.0, 1.0);
		glLineWidth(1);
		mTexture->unbind();
	}
}

void Photo::update()
{
	mTexture->loadColorBuffer(_width, _height, _buffer);
}	
