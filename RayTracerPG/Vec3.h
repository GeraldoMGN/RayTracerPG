#pragma once

#include<math.h>

class Vec3 {
private:
	double x, y, z;
public:
	Vec3(double x, double y, double z);
	Vec3(double xyz);

	double length() const;
	void normalize();

	double getX() const;
	double getY() const;
	double getZ() const;

	Vec3 operator+(const Vec3 rhs) const;
	Vec3 operator*(const double rhs) const;
	Vec3 operator*(const Vec3 rhs) const;
	Vec3 operator/(const double rhs) const;
};
