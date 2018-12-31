#include "AABBox.h"

AABBox::AABBox(Vec3* boundingPoint1, Vec3* boundingPoint2, Mesh* mesh, int maxLevel, int level) : maxLevel(maxLevel), level(level) {
	boundingPoints = new Vec3*[2];
	boundingPoints[0] = boundingPoint1;
	boundingPoints[1] = boundingPoint2;

	//Should get the triangles inside the region of this bound box
	if (level == maxLevel)
		faces = *getFacesInBox(boundingPoints, mesh);
	//or, if is not the last level of the octree, should create the subBoxes 
	else {
		//pointxyz where point111 is the boundingPoint1 and point333 is the boundingPoint2
		//point222 is half way in coordinates x,y,z between point111 and point333
		Vec3 halfWay((*boundingPoints[1] - *boundingPoints[0]) / 2);

		Vec3* point222 = new Vec3(*boundingPoints[0] + halfWay);
		Vec3* point211 = new Vec3(*boundingPoints[0] + Vec3(halfWay.getX(), 0.0, 0.0));
		Vec3* point322 = new Vec3(*point211 + halfWay);
		Vec3* point112 = new Vec3(*boundingPoints[0] + Vec3(0.0, 0.0, halfWay.getZ()));
		Vec3* point223 = new Vec3(*point112 + halfWay);
		Vec3* point212 = new Vec3(*boundingPoints[0] + Vec3(halfWay.getX(), 0.0, halfWay.getZ()));
		Vec3* point323 = new Vec3(*point212 + halfWay);
		Vec3* point121 = new Vec3(*boundingPoints[0] + Vec3(0.0, halfWay.getY(), 0.0));
		Vec3* point232 = new Vec3(*point121 + halfWay);
		Vec3* point221 = new Vec3(*boundingPoints[0] + Vec3(halfWay.getX(), halfWay.getY(), 0.0));
		Vec3* point332 = new Vec3(*point221 + halfWay);
		Vec3* point122 = new Vec3(*boundingPoints[0] + Vec3(0.0, halfWay.getY(), halfWay.getZ()));
		Vec3* point233 = new Vec3(*point122 + halfWay);

		subBoxes[0] = new AABBox(boundingPoints[0], point222, mesh, maxLevel, level + 1);
		subBoxes[1] = new AABBox(point211		  , point322, mesh, maxLevel, level + 1);
		subBoxes[2] = new AABBox(point112		  , point223, mesh, maxLevel, level + 1);
		subBoxes[3] = new AABBox(point212		  , point323, mesh, maxLevel, level + 1);
		subBoxes[4] = new AABBox(point121		  , point232, mesh, maxLevel, level + 1);
		subBoxes[5] = new AABBox(point221		  , point332, mesh, maxLevel, level + 1);
		subBoxes[6] = new AABBox(point122		  , point233, mesh, maxLevel, level + 1);
		subBoxes[7] = new AABBox(point222, boundingPoints[1], mesh, maxLevel, level + 1);
	}
}

bool AABBox::intersect(const Ray& ray, ObjectIntersection* info, Mesh* mesh) const
{
	if (faces.size() == 0 && level == maxLevel)
		return false;
	double txMin, txMax, tyMin, tyMax, tzMin, tzMax;
	Vec3 invertedDirection = ray.getDirection().inverted();
	int signs[3];
	signs[0] = (invertedDirection.getX() < 0);
	signs[1] = (invertedDirection.getY() < 0);
	signs[2] = (invertedDirection.getZ() < 0);

	txMin = (this->boundingPoints[    signs[0]]->getX() - ray.getOrigin().getX()) * invertedDirection.getX();
	txMax = (this->boundingPoints[1 - signs[0]]->getX() - ray.getOrigin().getX()) * invertedDirection.getX();
	tyMin = (this->boundingPoints[    signs[1]]->getY() - ray.getOrigin().getY()) * invertedDirection.getY();
	tyMax = (this->boundingPoints[1 - signs[1]]->getY() - ray.getOrigin().getY()) * invertedDirection.getY();

	if ((txMin > tyMax) || (tyMin > txMax))
		return false;
	if (tyMin > txMin)
		txMin = tyMin;
	if (tyMax < txMax)
		txMax = tyMax;

	tzMin = (boundingPoints[    signs[2]]->getZ() - ray.getOrigin().getZ()) * invertedDirection.getZ();
	tzMax = (boundingPoints[1 - signs[2]]->getZ() - ray.getOrigin().getZ()) * invertedDirection.getZ();

	if ((txMin > tzMax) || (tzMin > txMax))
		return false;
	if (tzMin > txMin)
		txMin = tzMin;
	if (tzMax < txMax)
		txMax = tzMax;

	if (level == maxLevel) {
		return mesh->intersect(ray, info, this->faces);
	}

	bool intersected = false;
	for (int i = 0; i < 8; i++) {
		ObjectIntersection* tempInfo = new ObjectIntersection{ DBL_MAX, Vec3(0.0f), Vec3(0.0f), info->o };
		bool intersectedTemp = this->subBoxes[i]->intersect(ray, tempInfo, mesh);
		if (intersectedTemp) {
			intersected = true;
			if (tempInfo->t < info->t) {
				*info = *tempInfo;
			}
		}
		delete tempInfo;
	}

	return intersected;
}

std::vector<const tinyobj::index_t*>* AABBox::getFacesInBox(Vec3** boundingPoints, Mesh* mesh) const
{
	//Should go through all the faces(sequence of tree vertices) and return a list of 
	//those who are inside de AABBox
	//Note: Maybe i can extract a method to create the vertices from intersect() and interpolateNormal()
	std::vector<const tinyobj::index_t*> verticesReturn = {};
	Vec3* vertex0 = nullptr;
	Vec3* vertex1 = nullptr;
	Vec3* vertex2 = nullptr;

	for (int i = 0; i < mesh->vertexIndexes.size() / 3; i++) {
		int index0 = mesh->vertexIndexes.at(i * 3).vertex_index;
		vertex0 = new Vec3(mesh->vertices.at(index0 * 3), mesh->vertices.at(index0 * 3 + 1), mesh->vertices.at(index0 * 3 + 2));
		int index1 = mesh->vertexIndexes.at(i * 3 + 1).vertex_index;
		vertex1 = new Vec3(mesh->vertices.at(index1 * 3), mesh->vertices.at(index1 * 3 + 1), mesh->vertices.at(index1 * 3 + 2));
		int index2 = mesh->vertexIndexes.at(i * 3 + 2).vertex_index;
		vertex2 = new Vec3(mesh->vertices.at(index2 * 3), mesh->vertices.at(index2 * 3 + 1), mesh->vertices.at(index2 * 3 + 2));

		Vec3 hit(0);
		if (AABBox::CheckLineBox(*boundingPoints[0] - Vec3(0.01), *boundingPoints[1] + Vec3(0.01), *vertex0, *vertex1, hit) ||
			AABBox::CheckLineBox(*boundingPoints[0] - Vec3(0.01), *boundingPoints[1] + Vec3(0.01), *vertex0, *vertex2, hit) ||
			AABBox::CheckLineBox(*boundingPoints[0] - Vec3(0.01), *boundingPoints[1] + Vec3(0.01), *vertex1, *vertex2, hit)) {
			verticesReturn.push_back(&mesh->vertexIndexes.at(i * 3));
			verticesReturn.push_back(&mesh->vertexIndexes.at(i * 3 + 1));
			verticesReturn.push_back(&mesh->vertexIndexes.at(i * 3 + 2));
		}

		delete vertex0;
		delete vertex1;
		delete vertex2;
	}
	return new std::vector<const tinyobj::index_t*>(verticesReturn);
}

inline int AABBox::GetIntersection(double fDst1, double fDst2, Vec3 P1, Vec3 P2, Vec3 &Hit) {
	if ((fDst1 * fDst2) >= 0.0f) return 0;
	if (fDst1 == fDst2) return 0;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}

inline int AABBox::InBox(Vec3 Hit, Vec3 B1, Vec3 B2, const int Axis) {
	if (Axis == 1 && Hit.getZ() > B1.getZ() && Hit.getZ() < B2.getZ() && Hit.getY() > B1.getY() && Hit.getY() < B2.getY()) return 1;
	if (Axis == 2 && Hit.getZ() > B1.getZ() && Hit.getZ() < B2.getZ() && Hit.getX() > B1.getX() && Hit.getX() < B2.getX()) return 1;
	if (Axis == 3 && Hit.getX() > B1.getX() && Hit.getX() < B2.getX() && Hit.getY() > B1.getY() && Hit.getY() < B2.getY()) return 1;
	return 0;
}

// returns true if line (L1, L2) intersects with the box (B1, B2)
// returns intersection point in Hit
inline int AABBox::CheckLineBox(Vec3 B1, Vec3 B2, Vec3 L1, Vec3 L2, Vec3 &Hit) {
	if (L2.getX() < B1.getX() && L1.getX() < B1.getX()) return false;
	if (L2.getX() > B2.getX() && L1.getX() > B2.getX()) return false;
	if (L2.getY() < B1.getY() && L1.getY() < B1.getY()) return false;
	if (L2.getY() > B2.getY() && L1.getY() > B2.getY()) return false;
	if (L2.getZ() < B1.getZ() && L1.getZ() < B1.getZ()) return false;
	if (L2.getZ() > B2.getZ() && L1.getZ() > B2.getZ()) return false;
	if (L1.getX() > B1.getX() && L1.getX() < B2.getX() &&
		L1.getY() > B1.getY() && L1.getY() < B2.getY() &&
		L1.getZ() > B1.getZ() && L1.getZ() < B2.getZ())
	{
		Hit = L1;
		return true;
	}
	if ((GetIntersection(L1.getX() - B1.getX(), L2.getX() - B1.getX(), L1, L2, Hit) && InBox(Hit, B1, B2, 1))
		|| (GetIntersection(L1.getY() - B1.getY(), L2.getY() - B1.getY(), L1, L2, Hit) && InBox(Hit, B1, B2, 2))
		|| (GetIntersection(L1.getZ() - B1.getZ(), L2.getZ() - B1.getZ(), L1, L2, Hit) && InBox(Hit, B1, B2, 3))
		|| (GetIntersection(L1.getX() - B2.getX(), L2.getX() - B2.getX(), L1, L2, Hit) && InBox(Hit, B1, B2, 1))
		|| (GetIntersection(L1.getY() - B2.getY(), L2.getY() - B2.getY(), L1, L2, Hit) && InBox(Hit, B1, B2, 2))
		|| (GetIntersection(L1.getZ() - B2.getZ(), L2.getZ() - B2.getZ(), L1, L2, Hit) && InBox(Hit, B1, B2, 3)))
		return true;

	return false;
}

const std::vector<const tinyobj::index_t*> AABBox::getVerticesIndices() const
{
	return this->faces;
}
