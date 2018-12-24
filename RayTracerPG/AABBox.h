#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Geometry.h"

#include <vector>

class AABBox {
private:
	//First point is always the one with the smaller x, y and z coordinates, and second has the bigger ones
	Vec3** boundingPoints;
	AABBox* subBoxes[8];
	int level;
	int maxLevel;
	std::vector<const tinyobj::index_t*> faces;

public:
	AABBox(Vec3* boundingPoint1, Vec3* boundingPoint2, Mesh* mesh, int maxLevel, int level);
	bool intersect(const Ray& r, ObjectIntersection* info, Mesh* mesh) const;

	const std::vector<const tinyobj::index_t*> getVerticesIndices() const;
};