#pragma once

#include "Vec3.h"
#include <float.h>

class Ray {
private:
	Vec3 origin, direction;
public:
	Ray(Vec3 origin, Vec3 direction);
	Vec3 getOrigin() const;
	Vec3 getDirection() const;
};
