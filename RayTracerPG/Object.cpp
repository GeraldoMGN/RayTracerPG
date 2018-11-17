#include "Object.h"

Object::Object(Geometry* geometry, Material* material) : geometry(geometry), material(material) {}

bool Object::intersect(const Ray& r, ObjectIntersection* info)
{
	info->o = this;
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
