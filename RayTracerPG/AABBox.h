#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Geometry.h"

#include <vector>

class AABBox {
private:
	Vec3* boundingPoints[2];
	AABBox* subBoxes[8];
	int level;
	int maxLevel;
	std::vector<tinyobj::index_t> faces;

public:
	AABBox(Vec3& boundingPoint1, Vec3& boundingPoint2, Mesh* mesh);
	const AABBox* intersect(const Ray& r) const;
};