#include "Geometry.h"

Sphere::Sphere(Vec3 center, double radius) : center(center), radius(radius){}

bool Sphere::intersect(const Ray & r, ObjectIntersection * info) const
{
	//TODO
	return false;
}

Vec3 Sphere::getPoint() const
{
	//TODO
	return Vec3(1.0f,1.0f,1.0f);
}
