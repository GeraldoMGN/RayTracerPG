#include "AABBox.h"
#include "Vec3.h"

AABBox::AABBox (Vec3& boundingPoint1, Vec3& boundingPoint2) {
	boundingPoints[0] = &boundingPoint1;
	boundingPoints[1] = &boundingPoint2;
}

bool AABBox::intersect(const Ray& ray) const
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

	return true;
}
