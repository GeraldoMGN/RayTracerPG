#include "Object.h"

Object::Object(Geometry* geometry, Material* materal) : geometry(geometry), material(material) {}

bool Object::intersect(const Ray& r, ObjectIntersection* info)
{
	return geometry->intersect(r, info);
}
