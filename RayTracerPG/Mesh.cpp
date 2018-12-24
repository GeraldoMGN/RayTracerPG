#include "Geometry.h"
#include "Consts.h"
#include <iostream>

Mesh::Mesh(std::vector<double>& vertices, std::vector<tinyobj::shape_t>& shapes, std::vector<double>& normals)
	: vertices(vertices), normals(normals)
{
	//já que o .obj é separado em grupos, o seguinte une todos os grupos num único mesh
	for (int i = 0; i < shapes.size(); i++) {
		tinyobj::mesh_t shape = shapes.at(i).mesh;
		for (int j = 0; j < shape.indices.size(); j++) {
			vertexIndexes.push_back(shape.indices.at(j));
		}
	}
}

bool Mesh::intersect(const Ray& ray, ObjectIntersection* info) const
{
	bool intersec = false;
	for (int i = 0; i < vertexIndexes.size() / 3; i++) {
		//Calcula as 3 vertices do triangulo
		int index0 = vertexIndexes.at(i * 3).vertex_index;
		Vec3* vertex0 = new Vec3(vertices.at(index0 * 3), vertices.at(index0 * 3 + 1), vertices.at(index0 * 3 + 2));
		int index1 = vertexIndexes.at(i * 3 + 1).vertex_index;
		Vec3* vertex1 = new Vec3(vertices.at(index1 * 3), vertices.at(index1 * 3 + 1), vertices.at(index1 * 3 + 2));
		int index2 = vertexIndexes.at(i * 3 + 2).vertex_index;
		Vec3* vertex2 = new Vec3(vertices.at(index2 * 3), vertices.at(index2 * 3 + 1), vertices.at(index2 * 3 + 2));

		ObjectIntersection* tempInfo = new ObjectIntersection{ DBL_MAX, Vec3(0.0f), Vec3(0.0f), info->o };
		double u, v;		//coordenadas baricentricas
		if (intersectTriangle(ray, vertex0, vertex1, vertex2, tempInfo, u, v))
			intersec = true;
		if (intersec && tempInfo->t < info->t) {
			*info = *tempInfo;
			if (normals.size() == 0)	//se não houver normais no .obj, calcula
				info->n = faceNormal(vertex0, vertex1, vertex2);
			else					//caso haja, usa interpolação para dar um resultado mais "Polido"
				info->n = interpolateNormal(i, u, v);
		}

		//Salvar memória
		delete[] tempInfo;
		delete[] vertex0;
		delete[] vertex1;
		delete[] vertex2;
	}
	return intersec;
}

bool Mesh::intersectTriangle(const Ray& ray, const Vec3* vertex0, const Vec3* vertex1, const Vec3* vertex2,
	ObjectIntersection* info, double& u, double& v)
{
	//Algoritmo de interseção de Möller–Trumbore
		//	Consiste em colocar o ponto de interseção em função das coordenadas baricentricas u e v,
		//e da distancia à origem t, com isso o espaço é definido na base t, u e v, em vez de x, y e z.
		//	Então a equação resultante é colocada em forma matricial e o seguinte algoritmo o resolve,
		//com usando regra de Cramer.
		//	Artigo original em: https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

	//As duas arestas ligadas ao vertice 0
	Vec3 edge0 = *vertex1 - *vertex0;
	Vec3 edge1 = *vertex2 - *vertex0;

	//calculo do determinante
	Vec3 pvec = ray.getDirection().crossProduct(edge1);
	double determinant = edge0.dotProduct(pvec);
	if (determinant > -EPSILON && determinant < EPSILON)	//Caso o determinante seja proximo de 0(Epsilon, para problemas 
		return false;										//de precisão de ponto flutuante)

	//calculo da coordenada baricentrica u
	Vec3 tvec = ray.getOrigin() - *vertex0;
	u = tvec.dotProduct(pvec) / determinant;
	if (u < 0.0 || u > 1.0)									//interseção fora do triangulo
		return false;

	//calculo da coordenada baricentrica v
	Vec3 qvec = tvec.crossProduct(edge0);
	v = ray.getDirection().dotProduct(qvec) / determinant;
	if (v < 0.0 || u + v > 1.0)								//interseção fora do triangulo
		return false;

	info->t = edge1.dotProduct(qvec) / determinant;
	if (info->t < 0)										//interseção atrás da camera
		return false;
	info->p = ray.getOrigin() + (ray.getDirection() * info->t);

	return true;
}

Vec3 Mesh::interpolateNormal(int& index, double & u, double & v) const
{
	//calculo das normais de cada vertice
	int normalIndex0 = vertexIndexes.at(index * 3 + 0).normal_index;
	Vec3* normal0 = new Vec3(normals.at(normalIndex0 * 3), normals.at(normalIndex0 * 3 + 1), normals.at(normalIndex0 * 3 + 2));
	int normalIndex1 = vertexIndexes.at(index * 3 + 1).normal_index;
	Vec3* normal1 = new Vec3(normals.at(normalIndex1 * 3), normals.at(normalIndex1 * 3 + 1), normals.at(normalIndex1 * 3 + 2));
	int normalIndex2 = vertexIndexes.at(index * 3 + 2).normal_index;
	Vec3* normal2 = new Vec3(normals.at(normalIndex2 * 3), normals.at(normalIndex2 * 3 + 1), normals.at(normalIndex2 * 3 + 2));

	//interpolação de acordo com as coordenadas baricentricas u e v
	Vec3 normal = Vec3((*normal0 * (1 - u - v)) + *normal1 * u + *normal2 * v);

	delete[] normal0;
	delete[] normal1;
	delete[] normal2;

	return normal;
}

Vec3 Mesh::faceNormal(Vec3* vertex0, Vec3* vertex1, Vec3* vertex2) const
{
	return (*vertex1 - *vertex0).crossProduct(*vertex2 - *vertex0).normalize();
}

std::vector<const tinyobj::index_t*>* Mesh::getFacesInBox(Vec3** boundingPoints) const
{
	//Should go through all the faces(sequence of tree vertices) and return a list of 
	//those who are inside de AABBox
	//Note: Maybe i can extract a method to create the vertices from intersect() and interpolateNormal()
	std::vector<const tinyobj::index_t*> verticesReturn = {};
	for (int i = 0; i < vertexIndexes.size() / 3; i++) {
		int index0 = vertexIndexes.at(i * 3).vertex_index;
		Vec3* vertex0 = new Vec3(vertices.at(index0 * 3), vertices.at(index0 * 3 + 1), vertices.at(index0 * 3 + 2));
		int index1 = vertexIndexes.at(i * 3 + 1).vertex_index;
		Vec3* vertex1 = new Vec3(vertices.at(index1 * 3), vertices.at(index1 * 3 + 1), vertices.at(index1 * 3 + 2));
		int index2 = vertexIndexes.at(i * 3 + 2).vertex_index;
		Vec3* vertex2 = new Vec3(vertices.at(index2 * 3), vertices.at(index2 * 3 + 1), vertices.at(index2 * 3 + 2));

		if (vertex0->getX() >= boundingPoints[0]->getX() && vertex0->getX() <= boundingPoints[1]->getX())
			if (vertex0->getY() >= boundingPoints[0]->getY() && vertex0->getY() <= boundingPoints[1]->getY())
				if (vertex0->getZ() >= boundingPoints[0]->getZ() && vertex0->getZ() <= boundingPoints[1]->getZ()) {
					verticesReturn.push_back(&vertexIndexes.at(i * 3));
					verticesReturn.push_back(&vertexIndexes.at(i * 3 + 1));
					verticesReturn.push_back(&vertexIndexes.at(i * 3 + 2));
					continue;
				}
		if (vertex1->getX() >= boundingPoints[0]->getX() && vertex1->getX() <= boundingPoints[1]->getX())
			if (vertex1->getY() >= boundingPoints[0]->getY() && vertex1->getY() <= boundingPoints[1]->getY())
				if (vertex1->getZ() >= boundingPoints[0]->getZ() && vertex1->getZ() <= boundingPoints[1]->getZ()) {
					verticesReturn.push_back(&vertexIndexes.at(i * 3));
					verticesReturn.push_back(&vertexIndexes.at(i * 3 + 1));
					verticesReturn.push_back(&vertexIndexes.at(i * 3 + 2));
					continue;
				}
		if (vertex2->getX() >= boundingPoints[0]->getX() && vertex2->getX() <= boundingPoints[1]->getX())
			if (vertex2->getY() >= boundingPoints[0]->getY() && vertex2->getY() <= boundingPoints[1]->getY())
				if (vertex2->getZ() >= boundingPoints[0]->getZ() && vertex2->getZ() <= boundingPoints[1]->getZ()) {
					verticesReturn.push_back(&vertexIndexes.at(i * 3));
					verticesReturn.push_back(&vertexIndexes.at(i * 3 + 1));
					verticesReturn.push_back(&vertexIndexes.at(i * 3 + 2));
					continue;
				}
	}
	return new std::vector<const tinyobj::index_t*>(verticesReturn);
}

Vec3* Mesh::getSmallerCoordinates() const
{
	double x = DBL_MAX, y = DBL_MAX, z = DBL_MAX;
	for (int i = 0; i < vertices.size(); i = i + 3) {
		if (vertices.at(i) < x)
			x = vertices.at(i);
		if (vertices.at(i + 1) < y)
			y = vertices.at(i + 1);
		if (vertices.at(i + 2) < z)
			z = vertices.at(i + 2);
	}
	return new Vec3(x, y, z);
}

Vec3* Mesh::getBiggerCoordinates() const
{
	double x = DBL_MIN, y = DBL_MIN, z = DBL_MIN;
	for (int i = 0; i < vertices.size(); i = i + 3) {
		if (vertices.at(i) > x)
			x = vertices.at(i);
		if (vertices.at(i + 1) > y)
			y = vertices.at(i + 1);
		if (vertices.at(i + 2) > z)
			z = vertices.at(i + 2);
	}
	return new Vec3(x, y, z);
}
