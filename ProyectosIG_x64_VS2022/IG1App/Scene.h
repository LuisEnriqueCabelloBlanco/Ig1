#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>
#include "Light.h"

class Scene
{
public:
	Scene() = default;
	~Scene()
	{
		free();
		resetGL();
	};

	Scene(const Scene& s) = delete;            // no copy constructor
	Scene& operator=(const Scene& s) = delete; // no copy assignment

	void init();

	void render(Camera const& cam) const;

	void update();

	void setScene(int it);

	void rotate();
	void orbit();

	void lightOnOff(bool onoff)
	{
		habemusLuz = onoff;
	}
protected:
	void free();
	void setGL();
	void resetGL();
	void makeScene0();
	void makeScene1();
	void makeScene2();
	void makeScene3();
	void makeScene4();
	void makeScene5();
	void makeScene6();
	void makeScene7();
	void sceneDirLight(Camera const& cam) const;


	Abs_Entity* buildCaza();

	Abs_Entity* caza;
	Abs_Entity* centro;
	double facingAngle = 0;
	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	int mId = 0;
	DirLight* dirLight = nullptr;
	bool habemusLuz = false;
};

#endif //_H_Scene_H_
;