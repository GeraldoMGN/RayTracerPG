#include "AABBox.h"

AABBox::AABBox(Vec3* boundingPoint1, Vec3* boundingPoint2, Mesh* mesh, int maxLevel, int level) : maxLevel(maxLevel), level(level) {
	boundingPoints = new Vec3*[2];
	boundingPoints[0] = boundingPoint1;
	boundingPoints[1] = boundingPoint2;

	//Should get the triangles inside the region of this bound box
	if (level == maxLevel)
		faces = *mesh->getFacesInBox(boundingPoints);
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
				delete[] tempInfo;
			}
		}

	}
	return intersected;
}

const std::vector<const tinyobj::index_t*> AABBox::getVerticesIndices() const
{
	return this->faces;
}
