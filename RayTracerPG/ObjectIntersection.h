#pragma once

#include "Vec3.h"

class Object;

struct ObjectIntersection {
	double t;
	Vec3 p, n;
	Object* o;
};