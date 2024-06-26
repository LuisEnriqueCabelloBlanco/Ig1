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

	void dirLightOnOff(bool onoff)
	{
		enableDirLight = onoff;
		if (enableDirLight) {
			dirLight->enable();
		}
		else {
			dirLight->disable();
		}
	}

	void posLightOnOff(bool onoff) {
		enablePosLight = onoff;
		if (enablePosLight) {
			posLight->enable();
		}
		else {
			posLight->disable();
		}
	}

	void spotLightOnOff(bool onoff) {
		enableSpotLight = onoff;
		if (enableSpotLight) {
			spotLight->enable();
		}
		else {
			spotLight->disable();
		}
	}

	void tieLightOnOff(bool onoff) {
		if (caza != nullptr) {
			enableTieLight = onoff;
			if (enableTieLight) {
				panza->enable();
			}
			else {
				panza->disable();
			}
		}
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
	void setLights();

	Abs_Entity* buildCaza();

	Abs_Entity* caza;
	Abs_Entity* centro;
	double facingAngle = 0;
	std::vector<Abs_Entity*> gObjects; // Entities (graphic objects) of the scene
	int mId = 0;
	DirLight* dirLight = nullptr;
	PosLight* posLight = nullptr;
	SpotLight* spotLight = nullptr;
	SpotLight* panza = nullptr;
	bool enableDirLight = true;
	bool enablePosLight = false;
	bool enableSpotLight = false;
	bool enableTieLight = false;
};

#endif //_H_Scene_H_
;