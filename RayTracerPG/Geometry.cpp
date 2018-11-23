#include "Geometry.h"
#include "Consts.h"

#include <iostream>

Sphere::Sphere(Vec3 center, double radius) : center(center), radius(radius){}

bool Sphere::intersect(const Ray& ray, ObjectIntersection* info) const
	//comentei tudo para eu entender heheheheh
{
	/*esse algoritmo é uma resolução geométrica para achar o parametro da equação parametrica(t)
	que representa onde o vetor fez a interseção com a esfera*/
	/*envolve achar o t que faz que chega mais perto do centro da esfera (tca) fazendo o produto
	escalar de L(vetor entre a origem do raio e o centro da esfera) e o vetor de direção, retornando
	a o tamanho do vetor L projetado no vetor de direção(tca)*/
	Vec3 L = this->center - ray.getOrigin();
	double tca = L.dotProduct(ray.getDirection());
	if (tca < 0.0f) {									//a interseção é atrás da camera
		return false;
	}
	/*sabendo o tca, temos que achar o thc que é onde o o parametro que corresponde ao meio caminho
	do vetor dentro da esfera, para então subitrair com o tca e achar o ponto de interseção do vetor 
	com a esfera mais próximo*/
	//triangulo: d(distancia do centro da esfera ao ponto mais proximo do vetor), raio e thc
	double dSquared = L.dotProduct(L) - tca * tca;		//pitagoras
	double rSquared = this->radius * this->radius;
	if (dSquared > rSquared)		//o vetor não passa pela esfera
		return false;
	double thc = sqrt(rSquared - dSquared);
	
	//pontos de interseção do vetor com a "casca" da esfera
	double t0 = tca - thc;
	double t1 = tca + thc;
	if (t0 < 0.0f && t1 < 0.0f) //ambas interseções atrás da camera 
		return false;
	
	//atribui os valores à estrutura de dados da interseção
	if (t0 > t1 && t1 > 0.0f)
		info->t = t1;
	if (t0 < t1 && t0 > 0.0f)
		info->t = t0;
	info->p = ray.getOrigin() + (ray.getDirection() * info->t);
	info->n = (info->p - center).normalize();
	return true;
}

Mesh::Mesh(std::vector<Vec3*> vertices, std::vector<long*> vertexIndexes, std::vector<Vec3*> normals) : vertices(vertices), vertexIndexes(vertexIndexes), normals(normals)
{
}

bool Mesh::intersect(const Ray& ray, ObjectIntersection* info) const
{
	bool intersec = false;
	for (int i = 0; i < vertexIndexes.size(); i++) {
		intersec = intersectTriangle(ray, vertices.at(0), vertices.at(1), vertices.at(2), normals.at(0), info);
		if (intersec) return intersec;
	}
	//intersec = intersectTriangle(ray, vertices.at(0), vertices.at(3), vertices.at(1), normals.at(1), info);
	return intersec;
}

bool Mesh::intersectTriangle(const Ray& ray, const Vec3* vertex0, const Vec3* vertex1, const Vec3* vertex2, const Vec3* normal, ObjectIntersection* info) const
{
	//baseado no algoritmo de interseção de Möller–Trumbore
	//std::cout << vertex1->getX() << std::endl;
	//As duas arestas ligadas ao vertice 0
	Vec3 edge0 = *vertex1 - *vertex0;
	Vec3 edge1 = *vertex2 - *vertex0;

	//calculo do determinante
	Vec3 pvec = ray.getDirection().crossProduct(edge1);
	double determinant = edge0.dotProduct(pvec);
	if (determinant > -EPSILON && determinant < EPSILON)
		return false;
	
	Vec3 tvec = ray.getOrigin() - *vertex0;
	double u = tvec.dotProduct(pvec) / determinant;
	if (u < 0.0 || u > u > 1.0)
		return false;

	Vec3 qvec = tvec.crossProduct(edge0);
	double v = ray.getDirection().dotProduct(qvec) / determinant;
	if (v < 0.0 || u + v > 1.0)
		return false;

	info->n = *normal;
	info->t = edge1.dotProduct(qvec) / determinant;
	if (info->t < 0)
		return false;
	info->p = ray.getOrigin() + (ray.getDirection() * info->t);
	return true;
}
