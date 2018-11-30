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

Mesh::Mesh(std::vector<double>& vertices, std::vector<tinyobj::shape_t>& shapes,
	std::vector<double>& normals)
	: vertices(vertices), normals(normals)
{
	//já que o .obj é separado em grupos, o seguinte une todos os grupos num único mesh
	for (int i = 0; i < shapes.size(); i++) {
		tinyobj::mesh_t shape = shapes.at(i).mesh;
		for (int j = 0; j < shape.indices.size(); j++) {
			vertexIndexes.push_back(shape.indices.at(j));
		}
		for (int j = 0; j < shape.num_face_vertices.size(); j++) {
			faceVertex.push_back(shape.num_face_vertices.at(j));
		}
	}
}

bool Mesh::intersect(const Ray& ray, ObjectIntersection* info) const
{
	bool intersec = false;
	for (int i = 0; i < faceVertex.size(); i++) {
		//Calcula as 3 vertices do triangulo
		int index0 = vertexIndexes.at(i * 3).vertex_index;
		Vec3* vertex0 = new Vec3(vertices.at(index0 * 3), vertices.at(index0 * 3 + 1), vertices.at(index0 * 3 + 2));
		int index1 = vertexIndexes.at(i * 3 + 1).vertex_index;
		Vec3* vertex1 = new Vec3(vertices.at(index1 * 3), vertices.at(index1 * 3 + 1), vertices.at(index1 * 3 + 2));
		int index2 = vertexIndexes.at(i * 3 + 2).vertex_index;
		Vec3* vertex2 = new Vec3(vertices.at(index2 * 3), vertices.at(index2 * 3 + 1), vertices.at(index2 * 3 + 2));

		ObjectIntersection* tempInfo = new ObjectIntersection{ DBL_MAX, Vec3(0.0f), Vec3(0.0f), info->o};
		double u, v;		//coordenadas baricentricas
		if (intersectTriangle(ray, vertex0, vertex1, vertex2, tempInfo, u, v))
			intersec = true;
		if (intersec && tempInfo->t < info->t) {
			*info = *tempInfo;
			if(normals.size() == 0)	//se não houver normais no .obj, calcula
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
