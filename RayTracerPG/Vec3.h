#pragma once

#include <math.h>

class Vec3 {
private:
	double x, y, z;
public:
	Vec3(double x, double y, double z);
	Vec3(double xyz);

	double length() const;
	Vec3 normalize() const;
	Vec3 reflect(const Vec3& axis) const;

	double getX() const;
	double getY() const;
	double getZ() const;

	Vec3 operator+(const Vec3 rhs) const;
	Vec3 operator-(const Vec3 rhs) const;
	Vec3 operator*(const double rhs) const;
	//multiplica cada componente do vetor pelo correspondente do outro vetor
	Vec3 operator*(const Vec3 rhs) const;
	Vec3 operator/(const double rhs) const;
	
	Vec3 crossProduct(const Vec3 rhs) const;
	double dotProduct(const Vec3 rhs) const;
};
