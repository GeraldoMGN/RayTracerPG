#include "Object.h"

#include <iostream>


Object::Object(Mesh* mesh, Material* material) : geometry(mesh), material(material) {
	Vec3* boundingPoint0 = mesh->getSmallerCoordinates();
	Vec3* boundingPoint1 = mesh->getBiggerCoordinates();
	bbox = new AABBox(boundingPoint0, boundingPoint1, mesh, 0, 0);
}

Object::Object(Sphere* sphere, Material* material) : geometry(sphere), material(material) {
}

bool Object::intersect(const Ray& r, ObjectIntersection* info)
{
	info->o = this;
	if (bbox != nullptr) {
		if (bbox->intersect(r) != nullptr)
			return geometry->intersect(r, info);
		else
			return false;
	} else
		return geometry->intersect(r, info);
}

Geometry * Object::getGeometry() const
{
	return this->geometry;
}

Material * Object::getMaterial() const
{
	return this->material;
}
