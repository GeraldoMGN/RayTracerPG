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

Vec3::Vec3(double xyz) : x(xyz), y(xyz), z(xyz) {}

double Vec3::length() const
{
	return sqrt(
		(this->x * this->x) +
		(this->y * this->y) +
		(this->z * this->z)
	);
}

Vec3 Vec3::normalize() const
{
	double length = this->length();
	double normalizedX = x / length;
	double normalizedY = y / length;
	double normalizedZ = z / length;
	return Vec3(normalizedX, normalizedY, normalizedZ);
}

Vec3 Vec3::inverted() const
{
	return Vec3(1 / this->x, 1 / this->y, 1 / this->z);
}

Vec3 Vec3::reflect(const Vec3& axis) const
{
	Vec3 rtn = *this - (axis * this->dotProduct(axis) * 2);
	return rtn;
}

Vec3 Vec3::operator+(const Vec3 rhs) const {
	return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3 Vec3::operator-(const Vec3 rhs) const
{
	return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3 Vec3::operator*(const double rhs) const {
	return Vec3(x * rhs, y * rhs, z * rhs);
}

Vec3 Vec3::operator*(const Vec3 rhs) const
{
	return Vec3(this->x * rhs.getX(), this->y * rhs.getY(), this->z * rhs.getZ());
}


Vec3 Vec3::operator/(const double rhs) const
{
	return Vec3(this->x / rhs, this->y / rhs, this->z / rhs);
}

Vec3 Vec3::crossProduct(const Vec3 rhs) const {
	return Vec3(y*rhs.getZ() - z * rhs.getY(),	//a2b3 − a3b2
		z*rhs.getX() - x * rhs.getZ(),			//a3b1 − a1b3
		x*rhs.getY() - y * rhs.getX());			//a1b2 − a2b1
}

double Vec3::dotProduct(const Vec3 rhs) const
{
	return (this->x * rhs.getX()) + (this->y * rhs.getY()) + (this->z * rhs.getZ());
}
