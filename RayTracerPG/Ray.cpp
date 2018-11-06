#include "Ray.h"

Ray::Ray(Vec3 origin, Vec3 direction) : origin(origin), direction(direction) {}

Vec3 Ray::getOrigin() const {
	return this->origin;
}

Vec3 Ray::getDirection() const {
	return this->direction;
}
