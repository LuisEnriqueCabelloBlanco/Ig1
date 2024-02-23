#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

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
protected:
	void free();
	void setGL();
	void resetGL();
	void makeScene0();
	void makeScene1();
	void makeScene2();
	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	int mId = 0;
};

#endif //_H_Scene_H_
;