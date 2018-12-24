#include "AABBox.h"
#include <iostream>

AABBox::AABBox(Vec3* boundingPoint1, Vec3* boundingPoint2, Mesh* mesh, int maxLevel, int level) : maxLevel(maxLevel) {
	boundingPoints[0] = boundingPoint1;
	boundingPoints[1] = boundingPoint2;
	std::cout << "Smaller: " << boundingPoints[0]->getX() << " " << boundingPoints[0]->getY() << " " << boundingPoints[0]->getZ() << std::endl;
	std::cout << "Bigger: " << boundingPoints[1]->getX() << " " << boundingPoints[1]->getY() << " " << boundingPoints[1]->getZ() << std::endl;

	//Should get the triangles inside the region of this bound box
	if (level == maxLevel)
		faces = mesh->getFacesInBox(*boundingPoints);
	//or, if is not the last level of the octree, should create the subBoxes 
	//else ()
}

const AABBox* AABBox::intersect(const Ray& ray) const
{
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
		return nullptr;
	if (tyMin > txMin)
		txMin = tyMin;
	if (tyMax < txMax)
		txMax = tyMax;

	tzMin = (boundingPoints[    signs[2]]->getZ() - ray.getOrigin().getZ()) * invertedDirection.getZ();
	tzMax = (boundingPoints[1 - signs[2]]->getZ() - ray.getOrigin().getZ()) * invertedDirection.getZ();

	if ((txMin > tzMax) || (tzMin > txMax))
		return nullptr;
	if (tzMin > txMin)
		txMin = tzMin;
	if (tzMax < txMax)
		txMax = tzMax;

	if (level == maxLevel)
		return this;

	const AABBox* subBox;
	for (int i = 0; i < 8; i++)
		subBox = this->subBoxes[i]->intersect(ray);
	return subBox;
}
