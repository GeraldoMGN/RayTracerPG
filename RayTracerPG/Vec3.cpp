#include "Vec3.h"

double Vec3::getX() const {
	return this->x;
}

double Vec3::getY() const {
	return this->y;
}

double Vec3::getZ() const {
	return this->z;
}

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator+(const Vec3 rhs) const {
	return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3 Vec3::operator*(const double rhs) const {
	return Vec3(x * rhs, y * rhs, z * rhs);
}

Vec3 Vec3::operator*(const Vec3 rhs) const {
	return Vec3(y*rhs.getZ() - z * rhs.getY(),	//a2b3 − a3b2
		z*rhs.getX() - x * rhs.getZ(),			//a3b1 − a1b3
		x*rhs.getY() - y * rhs.getX());			//a1b2 − a2b1
}
