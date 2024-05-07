#include "Material.h"
#include <glm/gtc/type_ptr.inl>
void Material::upload()
{
	glMaterialfv(face, GL_AMBIENT, glm::value_ptr(ambient));
	glMaterialfv(face, GL_DIFFUSE, glm::value_ptr(diffuse));
	glMaterialfv(face, GL_SPECULAR, glm::value_ptr(specular));
	glMaterialf(face, GL_SHININESS, expF);
	glShadeModel(sh);
	// glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE , GL_FALSE ); // Defecto
	
	//glColorMaterial(face, GL_AMBIENT); con estas dos se ve oscuro el material, pense que habia que poner todas las componentes de la luz pero resulta que no? tengo que preguntar sobre como va esto, tengo sueño
	//glColorMaterial(face, GL_DIFFUSE);
	glColorMaterial(face, GL_SPECULAR);
	glColorMaterial(face, GL_SHININESS);
}

void Material::setCopper()
{
	ambient = { 0.19125 , 0.0735 , 0.0225 , 1 };
	diffuse = { 0.7038 , 0.27048 , 0.0828 , 1 };
	specular = { 0.256777 , 0.137622 , 0.086014 , 1 };
	expF = 12.8;
}

void Material::setGold()
{
	ambient = { 0.24725 , 0.1999 , 0.0745 , 1 };
	diffuse = { 0.75164 , 0.60648 , 0.22648 , 1 };
	specular = { 0.0745 , 0.22648 , 0.366065 , 1 };
	expF = 51.2;
}
