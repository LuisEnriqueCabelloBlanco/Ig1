#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>
class Light
{
protected:
	static GLuint cont; // Atributo para poder generar un nuevo id cada vez
	GLuint id = GL_LIGHT0 + GL_MAX_LIGHTS; // Primer id no válido
	// Atributos lumínicos y geométrico de la fuente de luz
	glm::fvec4 ambient = { 0.1 , 0.1 , 0.1 , 1 };
	glm::fvec4 diffuse = { 0.5 , 0.5 , 0.5 , 1 };
	glm::fvec4 specular = { 0.5 , 0.5 , 0.5 , 1 };
	glm::fvec4 posDir = { 0 , 0 , 1 , 0 };
	// Añade setters para cambiar el valor de los atributos lumínicos
public:
	Light();
	virtual ~Light() { disable(); }
	void uploadL() const;
	// Método abstracto
	virtual void upload(glm::dmat4 const& modelViewMat) const = 0;

	inline void disable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) glDisable(id); };
	inline void enable() { if (id < GL_LIGHT0 + GL_MAX_LIGHTS) glEnable(id); };
	inline void setAmb(glm::fvec4 amb) {ambient = amb; uploadL();};
	inline void setDiff(glm::fvec4 diff) { diffuse = diff;uploadL(); };
	inline void setSpec(glm::fvec4 spec) { specular = spec;uploadL(); };
	inline void setPosDir(glm::fvec4 dir) { posDir = dir;uploadL(); };
	inline glm::fvec4 getPosDir() { return posDir; };
};

class DirLight : public Light {
public:
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	void setPosDir(glm::fvec3 dir);
};

class PosLight : public Light {
protected:
	// Factores de atenuación
	GLfloat kc_ = 1, kl_ = 0, kq_ = 0;
public:
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	void setPosDir(glm::fvec3 dir);
	inline void setAtte(GLfloat kc, GLfloat kl, GLfloat kq) { kc_ = kc; kl_ = kl;kq_ = kq; };
};

class SpotLight : public PosLight {
protected:
	// Atributos del foco
	glm::fvec4 direction = { 0 , 0 , -1 , 0 };
	GLfloat cutoff = 180;
	GLfloat exp = 0;
public:
	SpotLight(glm::fvec3 pos = { 0 , 0 , 0 }) : PosLight() {
		posDir = glm::fvec4(pos, 1.0);
	};
	virtual void upload(glm::dmat4 const& modelViewMat) const;
	void setSpot(glm::fvec3 dir, GLfloat cf, GLfloat e) { direction = glm::fvec4(dir, 0.0); cutoff = cf;exp = e; };
};
