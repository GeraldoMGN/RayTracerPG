#include "Scene.h"

void Scene::add(Object* object)
{
	this->objects.push_back(object);
}

int Scene::getNumberObjects()
{
	return (int) this->objects.size();
}

Object* Scene::getObject(int object)
{
	return this->objects.at(object);
}
