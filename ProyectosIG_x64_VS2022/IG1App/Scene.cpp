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
	case 4:
		makeScene4();
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
	glEnable(GL_COLOR_MATERIAL);
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
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
	poli3->setModelMat(glm::translate(poli3->modelMat(), glm::dvec3(0, r, 0)));
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

	auto grass = new Grass();
	Texture* textura2 = new Texture();
	textura2->load("../textures/grass.bmp", u8vec3(0, 0, 0), 0);
	grass->setMTexture(textura2);
	grass->setModelMat(glm::translate(grass->modelMat(), glm::dvec3(200, 25, 200)));
	gObjects.push_back(grass);

	auto star = new Star3D(200, 8, 200);
	gObjects.push_back(star);
	Texture* texture = new Texture();
	texture->load("../textures/baldosaP.bmp");
	star->setMTexture(texture);
	
	auto photo = new Photo(glutGet(GLUT_WINDOW_WIDTH)/5, glutGet(GLUT_WINDOW_HEIGHT)/5, GL_FRONT);
	Texture* textura4 = new Texture();
	textura4->loadColorBuffer(GLUT_SCREEN_WIDTH/2, GLUT_SCREEN_HEIGHT/2, GL_FRONT);
	photo->setMTexture(textura4);
	gObjects.push_back(photo);
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


	auto parapet = new GlassParapet(500);
	Texture* textura3 = new Texture();
	textura3->load("../textures/windowV.bmp",200U);
	parapet->setMTexture(textura3);
	gObjects.push_back(parapet);

}

void Scene::makeScene4()
{
	gObjects.clear();


	auto ground = new Ground(500, 500);
	Texture* textura = new Texture();
	textura->load("../textures/baldosaC.bmp");
	ground->setMTexture(textura);
	gObjects.push_back(ground);

	auto box = new Box(100);
	Texture* textura1 = new Texture();
	textura1->load("../textures/container.bmp");
	box->setMTexture(textura1);
	Texture* textura2 = new Texture();
	textura2->load("../textures/papelE.bmp");
	box->setMBackTexture(textura2);
	box->setModelMat(glm::translate(box->modelMat(), glm::dvec3(-199, 51, 199)));
	gObjects.push_back(box);

	auto star = new Star3D(50, 8, 25);
	Texture* texture = new Texture();
	texture->load("../textures/baldosaP.bmp");
	star->setMTexture(texture);
	star->setModelMat(glm::translate(star->modelMat(), glm::dvec3(-199, 150, 199)));
	gObjects.push_back(star);

	auto grass = new Grass();
	Texture* textura4 = new Texture();
	textura4->load("../textures/grass.bmp", u8vec3(0, 0, 0), 0);
	grass->setMTexture(textura4);
	grass->setModelMat(glm::translate(grass->modelMat(), glm::dvec3(200, 50, -200)));
	gObjects.push_back(grass);

	auto photo = new Photo(glutGet(GLUT_WINDOW_WIDTH) / 5, glutGet(GLUT_WINDOW_HEIGHT) / 5, GL_FRONT);
	Texture* textura5 = new Texture();
	textura5->loadColorBuffer(GLUT_SCREEN_WIDTH / 2, GLUT_SCREEN_HEIGHT / 2, GL_FRONT);
	photo->setMTexture(textura5);
	gObjects.push_back(photo);

	auto parapet = new GlassParapet(500);
	Texture* textura3 = new Texture();
	textura3->load("../textures/windowV.bmp", 200U);
	parapet->setMTexture(textura3);
	gObjects.push_back(parapet);
}
void
Scene::render(Camera const& cam) const
{
	sceneDirLight(cam);

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

void Scene::sceneDirLight(Camera const& cam) const {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glm::fvec4 posDir = { 1, 1, 1, 0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	glm::fvec4 ambient = { 0, 0, 0, 1 };
	glm::fvec4 diffuse = { 1, 1, 1, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));
}
