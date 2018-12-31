#pragma once

#include "ObjectIntersection.h"
#include "Vec3.h"
#include "Ray.h"
#include "cmath"

#define TINYOBJLOADER_USE_DOUBLE
#include "Include/tiny_obj_loader.h"

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
public:
	std::vector<double> vertices;
	std::vector<tinyobj::index_t> vertexIndexes;
	std::vector<double> normals;
	Mesh(std::vector<double>& vertices, std::vector<tinyobj::shape_t>& shapes, std::vector<double>& normal);
	
	//interseção com o mesh
	bool intersect(const Ray& r, ObjectIntersection* info = nullptr) const override;
	bool intersect(const Ray& r, ObjectIntersection* info, const std::vector<const tinyobj::index_t*>& verticesIndexes) const;
	//interseção com um triangulo individual
	static bool intersectTriangle(const Ray& ray, const Vec3* vertex0, const Vec3* vertex1, 
		const Vec3* vertex2, ObjectIntersection* info, double& u, double& v);

	//Returns the face normal based on the mesh indices
	Vec3 interpolateNormal(int& index, double& u, double& v) const;
	//Returns the face normal based on the bbox indices
	Vec3 interpolateNormal(const std::vector<const tinyobj::index_t*>& verticesIndexes, int& index, double& u, double& v) const;
	//Retorna a normal da face (usada caso não se tenha as normais no mesh)
	Vec3 faceNormal(Vec3* vertex0, Vec3* vertex1, Vec3* vertex2) const;

	Vec3* getSmallerCoordinates() const;
	Vec3* getBiggerCoordinates() const;
};
