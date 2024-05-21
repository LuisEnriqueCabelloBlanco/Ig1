#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"

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

class EntityWithMaterial : public Abs_Entity {
public:
	EntityWithMaterial() : Abs_Entity() { };
	virtual ~EntityWithMaterial() { delete material; };
	void setMaterial(Material* matl) { material = matl; };
protected:
	Material* material = nullptr;
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
	explicit Ground(GLdouble w, GLdouble h);
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

class Box : public Abs_Entity {
public:
	explicit Box(GLdouble length);
	~Box();
	virtual void render(glm::dmat4 const& modelViewMat) const override;
	virtual void update();
	Mesh* upperCase;
	glm::dmat4 mModelMatUpperCase;
	Mesh* underCase;
	glm::dmat4 mModelMatUnderCase;
	GLdouble mLength;
	GLdouble rotatedAngle;
	int rotationSing;
	GLdouble rotationSpeed;
};

class GlassParapet : public Abs_Entity {
public:
	explicit GlassParapet(int size);
	~GlassParapet();
	virtual void render(glm::dmat4 const& modelViewMat) const override;
};
class Grass : public Abs_Entity {
public:
	explicit Grass();
	~Grass();
	virtual void render(glm::dmat4 const& modelViewMat) const override;
};

class Photo : public Abs_Entity
{
protected:
	GLsizei _width, _height;
	GLuint _buffer;
public:
	explicit Photo(GLsizei width, GLsizei height, GLuint buffer);
	~Photo();
	virtual void render(glm::dmat4 const& modelViewMat) const override;
	virtual void update();
};

class QuadricEntity : public Abs_Entity {
public:
	QuadricEntity();
	~QuadricEntity() { gluDeleteQuadric(qEnt_); }
protected:
	GLUquadricObj* qEnt_;
};

class Sphere : public EntityWithMaterial {
public:
	Sphere(GLdouble r); // r es el radio de la esfera
	void render(glm::dmat4 const& modelViewMat) const override;
protected:
	GLdouble r;
	GLUquadricObj* qEnt_;
};

class Cylinder : public QuadricEntity
{
public:
	Cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, int slices, int stacks);
	void render(glm::dmat4 const& modelViewMat) const override;
protected:
	GLdouble baseRadius_;
	GLdouble topRadius_;
	GLdouble height_;
	int slices_;
	int stacks_;
};
class Disk : public QuadricEntity
{
public:
	Disk(GLdouble innerRadius, GLdouble outerRadius, int slices, int rings);
	void render(glm::dmat4 const& modelViewMat) const override;
protected:
	GLdouble innerRadius_;
	GLdouble outerRadius_;
	int slices_;
	int rings_;
};
class PartialDisk : public QuadricEntity
{
public:
	PartialDisk(GLdouble innerRadius, GLdouble outerRadius, int slices, int rings, GLdouble startAngle, GLdouble sweepAngle);
	void render(glm::dmat4 const& modelViewMat) const override;
protected:
	GLdouble innerRadius_;
	GLdouble outerRadius_;
	int slices_;
	int rings_;
	GLdouble startAngle_;
	GLdouble sweepAngle_;
};

class CompoundEntity : public Abs_Entity {
public:
	CompoundEntity();
	~CompoundEntity();
	void addEntity(Abs_Entity* ae);
	void render(glm::dmat4 const& modelViewMat) const override;
	void addLight(SpotLight* l) { light = l; };
protected:
	std::vector<Abs_Entity*> gObjects;
	SpotLight* light = nullptr;
};

class WingAdvancedTIE : public Abs_Entity {
public:
	WingAdvancedTIE();
	~WingAdvancedTIE();
protected:
	virtual void render(glm::dmat4 const& modelViewMat) const override;
};

class IndexedBox : public Abs_Entity {
public:
	IndexedBox(GLuint l);
	~IndexedBox();
protected:
	virtual void render(glm::dmat4 const& modelViewMat) const override;
};

class IndexSphere :public Abs_Entity {
public:
	IndexSphere(GLdouble radio, int p, int m);
	~IndexSphere();
protected:
	virtual void render(glm::dmat4 const& modelViewMat) const override;
};
class IndexToroid : public Abs_Entity {
public:
	IndexToroid(GLdouble circleRadio, GLdouble toroidRadio, int muestras, int points);
	~IndexToroid();
protected:
	virtual void render(glm::dmat4 const& modelViewMat) const override;
};

#endif //_H_Entities_H_
