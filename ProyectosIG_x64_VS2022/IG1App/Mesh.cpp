#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

void
Mesh::draw() const
{
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::render() const
{
	if (vVertices.size() > 0) { // transfer data
		// transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(
		  3, GL_DOUBLE, 0, vVertices.data()); // number of coordinates per vertex, type of
		                                      // each coordinate, stride, pointer
		if (vColors.size() > 0) {             // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(
			  4, GL_DOUBLE, 0, vColors.data()); // components number (rgba=4), type of
			                                    // each component, stride, pointer
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

Mesh* Mesh::generateRegularPolygon(GLuint num, GLdouble r)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP; //es la primitiva que nos piden
	mesh->mNumVertices = num;
	mesh->vVertices.reserve(mesh->mNumVertices); //reservar espacio para vertices y colores
	//mesh->vColors.reserve(mesh->mNumVertices);

	double x = 0, y = r, angle = 90; //se empieza en 90 porque lo pide el enunciado

	for (size_t i = 0; i < num; i++)
	{
		x = 0 + r * glm::cos(glm::radians(angle)); //formulitas
		y = 0 + r * glm::sin(glm::radians(angle));

		mesh->vVertices.emplace_back(x, y, 0.0);

		//mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);

		angle += 360.0 / num; //se avanza angulo
	}

	return mesh;
}

Mesh* Mesh::generateRegularPolygon(GLuint num, GLdouble r, std::vector<glm::dvec4>& colors)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLES; //es la primitiva que nos piden
	mesh->mNumVertices = num;
	mesh->vVertices.reserve(mesh->mNumVertices); //reservar espacio para vertices y colores
	mesh->vColors = colors;

	double x = 0, y = r, angle = 90; //se empieza en 90 porque lo pide el enunciado

	for (size_t i = 0; i < num; i++)
	{
		x = 0 + r * glm::cos(glm::radians(angle)); //formulitas
		y = 0 + r * glm::sin(glm::radians(angle));

		mesh->vVertices.emplace_back(x, y, 0.0);

		mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);

		angle += 360.0 / num; //se avanza angulo
	}

	return mesh;
}

Mesh* Mesh::generateRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back((w / 2), -(h / 2), 0.0);
	mesh->vVertices.emplace_back(-(w / 2), -(h / 2), 0.0);
	mesh->vVertices.emplace_back((w / 2), (h / 2), 0.0);
	mesh->vVertices.emplace_back(-(w / 2), (h / 2), 0.0);

	return mesh;
}

Mesh* Mesh::generateRGBRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = generateRectangle(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);

	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	return mesh;
}

//metodo auxiliar para crear las caras del cubo con dos triangulos (6 vertices)
Mesh* Mesh::generateSquare(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(-(l / 2), -(l / 2), 0.0);
	mesh->vVertices.emplace_back((l / 2), -(l / 2), 0.0);
	mesh->vVertices.emplace_back((l / 2), (l / 2), 0.0);
	mesh->vVertices.emplace_back(-(l / 2), -(l / 2), 0.0);
	mesh->vVertices.emplace_back((l / 2), (l / 2), 0.0);
	mesh->vVertices.emplace_back(-(l / 2), (l / 2), 0.0);

	return mesh;
}

Mesh* Mesh::generateCube(GLdouble length)
{

	Mesh* meshDeliver = new Mesh();
	meshDeliver->mPrimitive = GL_TRIANGLES;
	meshDeliver->mNumVertices = 36;
	meshDeliver->vVertices.reserve(meshDeliver->mNumVertices);
	//meshDeliver->vColors.reserve(meshDeliver->mNumVertices);

	Mesh* auxMesh =generateSquare(length);

	//frontal
	dmat4 auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,length/2));
	for (size_t i = 0; i < auxMesh->mNumVertices; i++)
	{
		meshDeliver->vVertices.emplace_back(auxTransMat * glm::vec4(auxMesh->vVertices[i].x, auxMesh->vVertices[i].y, auxMesh->vVertices[i].z, 1));
		//meshDeliver->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	}

	//derecha
	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(length/2, 0.0, 0.0));
	dmat4 auxRotMat = glm::rotate(auxTransMat, glm::radians(90.0), glm::dvec3(0.0, 1.0, 0.0));

	for (size_t i = 0; i < auxMesh->mNumVertices; i++)
	{
		meshDeliver->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh->vVertices[i].x, auxMesh->vVertices[i].y, auxMesh->vVertices[i].z, 1));
		//meshDeliver->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	}
	//izquierda
	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(-length / 2, 0.0, 0.0));
	auxRotMat = glm::rotate(auxTransMat, glm::radians(-90.0), glm::dvec3(0.0, 1.0, 0.0));

	for (size_t i = 0; i < auxMesh->mNumVertices; i++)
	{
		meshDeliver->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh->vVertices[i].x, auxMesh->vVertices[i].y, auxMesh->vVertices[i].z, 1));
		//meshDeliver->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	}
	//atras
	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -length/2));
	auxRotMat = glm::rotate(auxTransMat, glm::radians(180.0), glm::dvec3(0.0, 1.0, 0.0));

	for (size_t i = 0; i < auxMesh->mNumVertices; i++)
	{
		meshDeliver->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh->vVertices[i].x, auxMesh->vVertices[i].y, auxMesh->vVertices[i].z, 1));
		//meshDeliver->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	}	
	//arriba
	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, length / 2, 0.0));
	auxRotMat = glm::rotate(auxTransMat, glm::radians(-90.0), glm::dvec3(1.0, 0.0, 0.0));
	
	for (size_t i = 0; i < auxMesh->mNumVertices; i++)
	{
		meshDeliver->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh->vVertices[i].x, auxMesh->vVertices[i].y, auxMesh->vVertices[i].z, 1));
		//meshDeliver->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	}

	//abajo
	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -length / 2, 0.0));
	auxRotMat = glm::rotate(auxTransMat, glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));

	for (size_t i = 0; i < auxMesh->mNumVertices; i++)
	{
		meshDeliver->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh->vVertices[i].x, auxMesh->vVertices[i].y, auxMesh->vVertices[i].z, 1));
		//meshDeliver->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	}


	return meshDeliver;
}

Mesh* Mesh::generateRGBCubeTriangles(GLdouble length)
{
	Mesh* mesh = generateCube(length);
	mesh->vColors.reserve(mesh->mNumVertices);
	//front r
	for (int i = 0; i < 6; i++) {
		mesh->vColors.emplace_back(dvec4(1.0,0.0,0.0,1.0));
	}
	//right g
	for (int i = 0; i < 6; i++) {
		mesh->vColors.emplace_back(dvec4(0.0, 1.0, 0.0, 1.0));
	}
	//left g
	for (int i = 0; i < 6; i++) {
		mesh->vColors.emplace_back(dvec4(0.0, 1.0, 0.0, 1.0));
	}
	//back r
	for (int i = 0; i < 6; i++) {
		mesh->vColors.emplace_back(dvec4(1.0, 0.0, 0.0, 1.0));
	}
	//up b
	for (int i = 0; i < 6; i++) {
		mesh->vColors.emplace_back(dvec4(0.0, 0.0, 1.0, 1.0));
	}
	//down b
	for (int i = 0; i < 6; i++) {
		mesh->vColors.emplace_back(dvec4(0.0, 0.0, 1.0, 1.0));
	}
	return mesh;
}


Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
