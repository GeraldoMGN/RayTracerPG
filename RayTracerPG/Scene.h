#pragma once

#include "Object.h"
#include "Light.h"
#include <vector>

class Scene {
private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;
public:
	//Objetos.
	void addObject(Object* object);
	int getNumberObjects();
	Object* getObject(int object);
	//Luzes
	void addLight(Light* light);
	int getNumberLights();
	Light* getLight(int light);
};
