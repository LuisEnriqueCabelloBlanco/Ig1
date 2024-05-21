#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <vector>

class Mesh
{
public:
	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh

	Mesh() = default;
	virtual ~Mesh() = default;

	Mesh(const Mesh& m) = delete;            // no copy constructor
	Mesh& operator=(const Mesh& m) = delete; // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; }; // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };

	static Mesh* generateRegularPolygon(GLuint num, GLdouble r);
	static Mesh* generateRegularPolygon(GLuint num, GLdouble r,std::vector<glm::dvec4>& colors);
	static Mesh* generateRectangle(GLdouble w, GLdouble h);
	static Mesh* generateRGBRectangle(GLdouble w, GLdouble h);
	static Mesh* generateSquare(GLdouble l);
	static Mesh* generateCube(GLdouble length);
	static Mesh* generateRGBCubeTriangles(GLdouble length);
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h);
	static Mesh* generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	static Mesh* generateBoxOutline(GLdouble length);
	static Mesh* generateBoxOutlineTexCor(GLdouble length);
	static Mesh* generateStar3D(GLdouble re, GLuint np, GLdouble h);
	static Mesh* generateBox(GLdouble length);
	static Mesh* generateWingAdvancedTie();

protected:
	GLuint mPrimitive =
	  GL_TRIANGLES;          // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0; // number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices; // vertex array
	std::vector<glm::dvec4> vColors;   // color array
	std::vector<glm::dvec2> vTexture; // coordenadas textura array
	std::vector<glm::dvec3> vNormals; // normals array
	virtual void draw() const;
};

class IndexMesh : public Mesh {
protected:
	std::vector<int> vIndices; // tabla de índices
	GLuint nNumIndices = 0;
	void buildNormalVectors();
	
public:
	IndexMesh() { mPrimitive = GL_TRIANGLES; }
	~IndexMesh() {  };
	virtual void render() const;
	virtual void draw() const;

	static IndexMesh* generateIndexedBox(GLdouble l);
};

class MbR : public IndexMesh {
protected:
	int n;
	std::vector<glm::dvec3> perfil;
	int m;
public:
	MbR(int rev, std::vector<glm::dvec3>& perf, int verticesPerfil);
	~MbR() {};

	static MbR* generateIndexMbR(int mm, int nn, std::vector<glm::dvec3>& perfil);
};
#endif //_H_Scene_H_
