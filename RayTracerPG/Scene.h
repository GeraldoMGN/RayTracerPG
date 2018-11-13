#pragma once

#include "Object.h"
#include <vector>

class Scene {
private:
	std::vector<Object*> objects;
public:
	//Adiciona um objeto a cena.
	void add(Object* object);
	int getNumberObjects();
	Object* getObject(int object);
};
