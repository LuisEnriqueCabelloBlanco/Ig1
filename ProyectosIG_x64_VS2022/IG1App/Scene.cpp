#include "Scene.h"
#include "CheckML.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Scene::init()
{
	setGL(); // OpenGL settings

	// allocate memory and load resources
	// Lights
	// Textures

	// Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));
	//RegularPolygon* poli = new RegularPolygon(3, 200);
	//poli->setMColor(dvec4(0.0, 1.0, 1.0, 1.0));
	//gObjects.push_back(poli); //n lados, r radio en pixeles

	/*RegularPolygon* poli2 = new RegularPolygon(40, 200);
	poli2->setMColor(dvec4(1.0, 0.0, 1.0, 1.0));
	gObjects.push_back(poli2); //n lados, r radio en pixeles*/

	/*RGBTriangle* poli3 = new RGBTriangle();
	gObjects.push_back(poli3);*/

	/*RGBRectangle* poli4 = new RGBRectangle(200, 100);
	gObjects.push_back(poli4);*/

	Cube* poli5 = new Cube(200);
	gObjects.push_back(poli5);
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
}
void
Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0); // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);     // disable Depth test
}

void
Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects) {
		el->render(cam.viewMat());
	}
}
