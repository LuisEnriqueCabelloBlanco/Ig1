#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Mesh.h"

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0), mColor(1), mTexture(nullptr),mBackTexture(nullptr) {}; // 4x4 identity matrix
	virtual ~Abs_Entity() = default;

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0; // abstract method
	virtual void update();


	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };

	glm::dvec4 const& getMColor() { return mColor; }
	void setMColor(glm::dvec4 const& color) { mColor = color; }

	Texture* const& getMTexture() { return mTexture; }
	void setMTexture(Texture* texture) { mTexture = texture; }
	
	Texture* const& getMBackTexture() { return mBackTexture; }
	void setMBackTexture(Texture* texture) { mBackTexture = texture; }

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::dmat4 mModelMat;  // modeling matrix
	glm::dvec4 mColor ;  // color vector
	Texture* mTexture; //texure / fonrt texture pointer
	Texture* mBackTexture; //back culling Texture pointer

	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
};

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class RegularPolygon : public Abs_Entity
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r);
	~RegularPolygon();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class RGBTriangle : public Abs_Entity
{
public:
	explicit RGBTriangle();
	~RGBTriangle();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
};

class RGBRectangle :public Abs_Entity {
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	~RGBRectangle();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Cube :public Abs_Entity {
public:
	explicit Cube(GLdouble length);
	~Cube();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class RGBCube : public Abs_Entity {
public:
	explicit RGBCube(GLdouble length);
	~RGBCube();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
private:
	double rotSpeed = 0.05;
	glm::f64 rotatedAngles = 0;
	enum states { rotX, rotZ, rotY};
	states currentState = rotX;
};

class Ground : public Abs_Entity {
public:
	explicit Ground();
	~Ground();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class BoxOutline : public Abs_Entity {
public:
	explicit BoxOutline(GLdouble length);
	~BoxOutline();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Star3D : public Abs_Entity {
public:
	explicit Star3D(GLdouble re, GLuint np, GLdouble h);
	~Star3D();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
};
#endif //_H_Entities_H_
