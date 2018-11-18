#include "Scene.h"

void Scene::addObject(Object* object)
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

void Scene::addLight(Light* light)
{
	this->lights.push_back(light);
}

int Scene::getNumberLights()
{
	return (int)this->lights.size();
}

Light* Scene::getLight(int light)
{
	return this->lights.at(light);
}

