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
		if (vTexture.size() > 0) {             // transfer colors
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexture.data());
		}
		if (vNormals.size() > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
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

	delete auxMesh;
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

Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h)
{
	Mesh* mesh = generateRectangle(w, h);

	mesh->vColors.reserve(mesh->mNumVertices);
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		mesh->vColors.emplace_back(dvec4(1.0, 1.0, 1.0, 1.0));
	}

	mesh->vTexture.reserve(mesh->mNumVertices);
	mesh ->vTexture.emplace_back(dvec2(1.0, 0.0));
	mesh ->vTexture.emplace_back(dvec2(0.0, 0.0));
	mesh ->vTexture.emplace_back(dvec2(1.0, 1.0));
	mesh ->vTexture.emplace_back(dvec2(0.0, 1.0));

	return mesh;
}

Mesh* Mesh::generateRectangleTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh)
{
	Mesh* mesh = generateRectangleTexCor(w, h);

	/*
	* Al estar en GL_Repeat lo que hacemos es multiplicar la posici�n de las cordenadas de
	* la textura y as� conseguimos que se repita las veces que queremos
	*/
	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vTexture[i].x *= rw;
		mesh->vTexture[i].y *= rh;
	}

	return mesh;
}

Mesh* Mesh::generateBoxOutline(GLdouble length)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 10;
	mesh->vVertices.reserve(mesh->mNumVertices);

	vector<dvec3> aux = {
		dvec3(length / 2, length / 2, -length / 2),
		dvec3(length / 2, -length / 2, -length / 2),
		dvec3(-length / 2, -length / 2, -length / 2),
		dvec3(-length / 2, length / 2, -length / 2),

		dvec3(length / 2, length / 2, length / 2),
		dvec3(length / 2, -length / 2, length / 2),
		dvec3(-length / 2, -length / 2, length / 2),
		dvec3(-length / 2, length / 2, length / 2),
	};

	
	mesh->vVertices.emplace_back(aux[2]);
	mesh->vVertices.emplace_back(aux[3]);
	mesh->vVertices.emplace_back(aux[1]);
	mesh->vVertices.emplace_back(aux[0]);
	mesh->vVertices.emplace_back(aux[5]);
	mesh->vVertices.emplace_back(aux[4]);
	mesh->vVertices.emplace_back(aux[6]);
	mesh->vVertices.emplace_back(aux[7]);
	mesh->vVertices.emplace_back(aux[2]);
	mesh->vVertices.emplace_back(aux[3]);

	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vColors.emplace_back(dvec4(1.0, 1.0, 1.0, 1.0));
	}

	return mesh;
}

Mesh* Mesh::generateBoxOutlineTexCor(GLdouble length)
{
	Mesh* mesh = generateBoxOutline(length);
	mesh->vTexture.reserve(mesh->mNumVertices);
	mesh->vTexture.emplace_back(dvec2(0.0,0.0));
	mesh->vTexture.emplace_back(dvec2(1.0, 0.0));
	mesh->vTexture.emplace_back(dvec2(0.0,1.0));
	mesh->vTexture.emplace_back(dvec2(1.0,1.0));	
	mesh->vTexture.emplace_back(dvec2(0.0,0.0));
	mesh->vTexture.emplace_back(dvec2(1.0, 0.0));
	mesh->vTexture.emplace_back(dvec2(0.0,1.0));
	mesh->vTexture.emplace_back(dvec2(1.0,1.0));
	mesh->vTexture.emplace_back(dvec2(0.0, 0.0));
	mesh->vTexture.emplace_back(dvec2(1.0, 0.0));
	return mesh;
}

Mesh* Mesh::generateStar3D(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();

	vector<dvec2> texCoords = {
		dvec2(1,0.75),
		dvec2(1,1),
		dvec2(0.75,1),
		dvec2(0.5,1),
		dvec2(0.25,1),
		dvec2(0,1),
		dvec2(0,0.75),
		dvec2(0,0.5),
		dvec2(0,0.25),
		dvec2(0,0),
		dvec2(0.25,0),
		dvec2(0.5,0),
		dvec2(0.75,0),
		dvec2(1,0),
		dvec2(1,0.25),
		dvec2(1,0.5)
	};
	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = np * 2 + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vTexture.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vTexture.emplace_back(0.5, 0.5);

	int radius;
	double step = 360.0 / (np * 2.0);
	double x;
	double y;
	double angle;
	//caculo vertices poligono estrellado
	for (int i = 0; i < np * 2; i++) {
		if (i % 2 == 0) radius = re/2;
		else radius = re;
		angle = i * step;

		x = 0 + radius * glm::cos(glm::radians(angle)); //formulitas
		y = 0 + radius * glm::sin(glm::radians(angle));

		mesh->vVertices.emplace_back(x, y, h);
		mesh->vTexture.emplace_back(texCoords[i % texCoords.size()]);
	}
	x = 0 + re/2 * glm::cos(glm::radians(0.0)); //formulitas
	y = 0 + re/2 * glm::sin(glm::radians(0.0));
	mesh->vVertices.emplace_back(x, y, h);
	return mesh;
}

Mesh* Mesh::generateBox(GLdouble length)
{
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 18;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vTexture.reserve(mesh->mNumVertices);

	Mesh* auxMesh1 = generateBoxOutlineTexCor(length);

	for (int i = 0; i < auxMesh1->mNumVertices; i++)
	{

		mesh->vVertices.emplace_back(auxMesh1->vVertices[i]);
		mesh->vColors.emplace_back(auxMesh1->vColors[i]);
		mesh->vTexture.emplace_back(auxMesh1->vTexture[i]);
	}

	Mesh* auxMesh2 = generateRectangleTexCor(length, length);
	dmat4 auxTransMat, auxRotMat;

	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -length / 2, 0.0));
	auxRotMat = glm::rotate(auxTransMat, glm::radians(90.0), glm::dvec3(1.0, 0.0, 0.0));
	for (int i = 0; i < auxMesh2->mNumVertices; i++)
	{

		mesh->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh2->vVertices[i].x, auxMesh2->vVertices[i].y, auxMesh2->vVertices[i].z, 1));
		mesh->vColors.emplace_back(auxMesh2->vColors[i]);
		mesh->vTexture.emplace_back(auxMesh2->vTexture[i]);
	}

	auxTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, length / 2, 0.0));
	auxRotMat = glm::rotate(auxTransMat, glm::radians(-90.0), glm::dvec3(1.0, 0.0, 0.0));
	for (int i = 0; i < auxMesh2->mNumVertices; i++)
	{

		mesh->vVertices.emplace_back(auxRotMat * glm::vec4(auxMesh2->vVertices[i].x, auxMesh2->vVertices[i].y, auxMesh2->vVertices[i].z, 1));
		mesh->vColors.emplace_back(auxMesh2->vColors[i]);
		mesh->vTexture.emplace_back(auxMesh2->vTexture[i]);
	}

	return mesh; //la caja verse se ve como se quiere, como se va a hacer el update? pues ni puta idea no se si es posible habiendolo hecho asi, no se tengo sue�o
}

Mesh* Mesh::generateWingAdvancedTie()
{
	//de hecho en las pelis las alas tienen esta forma, no son completamente rectangulares
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(dvec3(0, 100,40));
	mesh->vVertices.emplace_back(dvec3(0, 100, -40));
	mesh->vVertices.emplace_back(dvec3(50, 50, 80));
	mesh->vVertices.emplace_back(dvec3(50, 50, -80));
	mesh->vVertices.emplace_back(dvec3(50, -50, 80));
	mesh->vVertices.emplace_back(dvec3(50, -50, -80));
	mesh->vVertices.emplace_back(dvec3(0, -100, 40));
	mesh->vVertices.emplace_back(dvec3(0, -100, -40));

	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vColors.emplace_back(dvec4(1.0, 1.0, 1.0, 1.0));
	}

	mesh->vTexture.reserve(mesh->mNumVertices);
	mesh->vTexture.emplace_back(dvec2(0,0));
	mesh->vTexture.emplace_back(dvec2(1,0));
	mesh->vTexture.emplace_back(dvec2(0,0.3));
	mesh->vTexture.emplace_back(dvec2(1,0.3));
	mesh->vTexture.emplace_back(dvec2(0,0.6));
	mesh->vTexture.emplace_back(dvec2(1,0.6));
	mesh->vTexture.emplace_back(dvec2(0,1));
	mesh->vTexture.emplace_back(dvec2(1,1));
	return mesh;
}

MbR::MbR(int rev, std::vector<dvec3>& perf, int verticesPerfil):IndexMesh()
{
	n = rev;
	perfil = perf;
	m = verticesPerfil;
}

MbR* MbR::generateIndexMbR(int mm, int mn, std::vector<glm::dvec3>& perfil)
{
	MbR* mesh = new MbR(mm, perfil,mn);
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->mNumVertices = mm * mn;
	std::vector<glm::dvec3> aux;
	aux.reserve(mesh->mNumVertices);
	for (int i = 0; i < mn; i++) {
		GLdouble theta = i * (360 / mn);
		GLdouble c = cos(glm::radians(theta));
		GLdouble s = sin(glm::radians(theta));
		for (int j = 0; j < mm; j++) {
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;
			//glm::dvec3 vector = glm::rotate<glm::dvec3>(perfil[j],glm::radians(theta),glm::dvec3(0.0,1.0,0.));
			aux.emplace_back(dvec3(x, perfil[j].y, z));
		}
	}
	mesh->vVertices = aux;
	mesh->mNumVertices = aux.size();
	int indiceMayor = 0;
	for (int i = 0; i < mn; i++) {
		// El contador j recorre los v�rtices del perfil ,
		// de abajo arriba . Las caras cuadrangulares resultan
		// al unir la muestra i- �sima con la (i +1)% nn - �sima

		for (int j = 0; j < mm - 1; j++) {
			// El contador indice sirve para llevar cuenta
			// de los �ndices generados hasta ahora . Se recorre
			// la cara desde la esquina inferior izquierda
			int indice = i * mm + j;
			mesh->vIndices.push_back(indice);
			indiceMayor++;
			mesh->vIndices.push_back((indice + mm)%(mn*mm));
			indiceMayor++;
			mesh->vIndices.push_back((indice + mm+1)%(mn*mm));
			indiceMayor++;



			mesh->vIndices.push_back(indice);
			indiceMayor++;
			mesh->vIndices.push_back((indice + mm + 1) % (mn * mm));
			indiceMayor++;
			mesh->vIndices.push_back(indice +1);
			indiceMayor++;
		}
	}
	mesh->nNumIndices = indiceMayor;
	mesh->buildNormalVectors();
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

void IndexMesh::buildNormalVectors()
{
	for(int i = 0; i < mNumVertices; i++)
	{
		vNormals.push_back(dvec3(0, 0, 0)); 
	}

	for(int i = 0; i < nNumIndices; i = i + 3)
	{
		dvec3 v1 = vVertices[vIndices[i]];
		dvec3 v2 = vVertices[vIndices[i + 1]];
		dvec3 v3 = vVertices[vIndices[i + 2]];

		dvec3 faceNormal = normalize(cross((v2 - v1), (v3 - v1)));

		vNormals[vIndices[i]] = vNormals[vIndices[i]] + faceNormal;
		vNormals[vIndices[i + 1]] = vNormals[vIndices[i + 1]] + faceNormal;
		vNormals[vIndices[i + 2]] = vNormals[vIndices[i + 2]] + faceNormal;
	}
	for (int i = 0; i < mNumVertices; i++)
	{
		vNormals[i] = normalize(vNormals[i]);
	}

}

void IndexMesh::render() const {


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
		if (vTexture.size() > 0) {             // transfer colors
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexture.data());
		}
		if (vNormals.size() > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}

		if (vIndices.size() > 0) { //para enviar datos a gpu
			glEnableClientState(GL_INDEX_ARRAY); 
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices.data());
		}
		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY); //acordarse siempre de desactivar lo que se ha activado ("opengl es un maquina de estados mimimimi" ya lo se luis pesao que solo dices eso)
	}
}

void IndexMesh::draw() const {
	glDrawElements(mPrimitive, nNumIndices,
		GL_UNSIGNED_INT, vIndices.data()); //modo de primitivas, num de elems a dibujar, tipo de los indices, indices
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l)
{
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->nNumIndices = 36;
	mesh->mNumVertices = 8;
	mesh->vNormals.reserve(mesh->mNumVertices);

	mesh->vVertices = {
		dvec3(l,l,l)*0.5, //0
		dvec3(l,-l,l) * 0.5, //1 
		dvec3(-l,-l,l) * 0.5, //2
		dvec3(-l,l,l) * 0.5, //3
		dvec3(-l,l,-l) * 0.5, //4
		dvec3(-l,-l,-l) * 0.5, //5 
		dvec3(l,-l,-l) * 0.5, //6
		dvec3(l,l,-l) * 0.5 //7
	};

	for (int i = 0; i < mesh->mNumVertices; i++) {
		mesh->vColors.push_back(dvec4(0.0, 1.0, 0.0, 1.0));
	}

	mesh->vIndices = 
	{
		0,1,7,
		1,6,7,

		3,2,1,
		3,1,0,

		4,5,2,
		4,2,3,

		7,6,5,
		7,5,4,

		4,3,0,
		4,0,7,

		5,1,2,
		5,6,1

	};

	mesh->buildNormalVectors();

	return mesh;
}

