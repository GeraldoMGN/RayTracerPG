#pragma once

#include "Vec3.h"
#include "Ray.h"

class AABBox {
private:
	Vec3* boundingPoints[2];
	int level;
	int maxLevel;

public:
	AABBox(Vec3& boundingPoint1, Vec3& boundingPoint2);
	bool intersect(const Ray& r) const;
};