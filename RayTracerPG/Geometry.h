#pragma once

#include "ObjectIntersection.h"
#include "Vec3.h"
#include "Ray.h"
#include "cmath"

#include <vector>

class Geometry {
public:
	/*Retorna true se r intersecta a geometria e falso caso contrário.
	Guarda possíveis informações sobre a intersecção em info, caso seja provido.*/
	virtual bool intersect(const Ray& ray, ObjectIntersection* info = nullptr) const = 0;
};

class Sphere : public Geometry {
private:
	Vec3 center;
	double radius;
public:
	Sphere(Vec3 center, double radius);
	bool intersect(const Ray& ray, ObjectIntersection* info = nullptr) const override;
};

class Mesh : public Geometry {
private:
	std::vector<Vec3*> vertices;
	std::vector<long*> vertexIndexes;
	std::vector<Vec3*> normals;
public:
	Mesh(std::vector<Vec3*> vertices, std::vector<long*> vertexIndexes, std::vector<Vec3*> faces);
	bool intersect(const Ray& r, ObjectIntersection* info = nullptr) const override;
	bool intersectTriangle(const Ray& ray, const Vec3* vertex0, const Vec3* vertex1, const Vec3* vertex2, const Vec3* normal, ObjectIntersection* info) const;
};