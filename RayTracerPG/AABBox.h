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

	std::vector<const tinyobj::index_t*>* getFacesInBox(Vec3** boundingPoints, Mesh* mesh) const;
	static int inline GetIntersection(double fDst1, double fDst2, Vec3 P1, Vec3 P2, Vec3 &Hit);
	static int inline InBox(Vec3 Hit, Vec3 B1, Vec3 B2, const int Axis);
	static int CheckLineBox(Vec3 B1, Vec3 B2, Vec3 L1, Vec3 L2, Vec3 &Hit);

	const std::vector<const tinyobj::index_t*> getVerticesIndices() const;
};