#include "Scene.h"
#include "CheckML.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"

using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene

	switch(mId)
	{
	case 0:
	    makeScene0();
		break;
	case 1:
		makeScene1();
		break;
	case 2:
		 makeScene2();
		break;
	case 3:
		 makeScene3();
		break;
		default:
			break;
	}
	//gObjects.push_back(new EjesRGB(400.0));
	//RegularPolygon* poli = new RegularPolygon(3, 200);
	//poli->setMColor(dvec4(0.0, 1.0, 1.0, 1.0));
	//gObjects.push_back(poli); //n lados, r radio en pixeles

	//RegularPolygon* poli2 = new RegularPolygon(40, 200);
	//poli2->setMColor(dvec4(1.0, 0.0, 1.0, 1.0));
	//gObjects.push_back(poli2); //n lados, r radio en pixeles

	//RGBTriangle* poli3 = new RGBTriangle();
	//gObjects.push_back(poli3);

	//RGBRectangle* poli4 = new RGBRectangle(400, 200);
	//gObjects.push_back(poli4);

	//Cube* poli5 = new Cube(200);
	//gObjects.push_back(poli5);

	/*RGBCube* poli6 = new RGBCube(200);
	gObjects.push_back(poli6);*/
}
void
Scene::free()
{ // release memory and resources

	for (Abs_Entity* el : gObjects) {
		delete el;
		el = nullptr;
	}
}
void
Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.6, 0.7, 0.8, 1.0); // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);          // enable Depth test
	glEnable(GL_TEXTURE_2D);
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
	glDisable(GL_TEXTURE_2D);
}

void Scene::makeScene0()
{
	gObjects.clear();
	gObjects.push_back(new EjesRGB(400.0));

	int r = 200;

	RegularPolygon* poli2 = new RegularPolygon(40, r);
	poli2->setMColor(dvec4(1.0, 0.0, 1.0, 1.0));
	gObjects.push_back(poli2); //n lados, r radio en pixeles

	RGBTriangle* poli3 = new RGBTriangle();
	poli3->setModelMat(glm::translate(poli3->modelMat(), glm::dvec3(r, 0, 0)));
	gObjects.push_back(poli3);

	RGBRectangle* poli4 = new RGBRectangle(400, 200);
	gObjects.push_back(poli4);
}

void Scene::makeScene1()
{
	gObjects.clear();
	gObjects.push_back(new EjesRGB(400.0));
	double cubeSize = 200;
	RGBCube* cubaso = new RGBCube(cubeSize);
	cubeSize = cubeSize / 2;
	cubaso->setModelMat(glm::translate(cubaso->modelMat(), glm::dvec3(cubeSize, cubeSize, -cubeSize)));
	gObjects.push_back(cubaso);
}

void Scene::makeScene2() {
	gObjects.clear();
	gObjects.push_back(new EjesRGB(400.0));

	auto ground = new Ground(500,500);
	Texture* textura = new Texture();
	textura->load("../textures/baldosaC.bmp");
	ground->setMTexture(textura);
	gObjects.push_back(ground);

	auto box = new BoxOutline(200);
	Texture* textura2 = new Texture();
	Texture* texturaBack = new Texture();
	textura2->load("../textures/container.bmp");
	texturaBack->load("../textures/papelE.bmp");
	box->setMTexture(textura2);
	box->setMBackTexture(texturaBack);
	gObjects.push_back(box);

	auto star = new Star3D(200, 8, 200);
	gObjects.push_back(star);
	Texture* texture = new Texture();
	texture->load("../textures/baldosaP.bmp");
	star->setMTexture(texture);


}

void Scene::makeScene3()
{
	gObjects.clear();
	gObjects.push_back(new EjesRGB(400.0));

	auto box = new Box(200);
	Texture* textura = new Texture();
	textura->load("../textures/container.bmp");
	box->setMTexture(textura);
	Texture* textura2 = new Texture();
	textura2->load("../textures/papelE.bmp");
	box->setMBackTexture(textura2);
	gObjects.push_back(box);
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects) {
		el->render(cam.viewMat());
	}
}

void Scene::update()
{
	for (auto obj : gObjects) {
		obj->update();
	}
}

void Scene::setScene(int id)
{
	mId = id;
	free();
	init();
}
