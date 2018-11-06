#pragma once

class Vec3 {
private:
	double x, y, z;
public:
	double getX() const;
	double getY() const;
	double getZ() const;
	Vec3(double x, double y, double z);
	Vec3 operator+(const Vec3 rhs) const;
	Vec3 operator*(const double rhs) const;
	Vec3 operator*(const Vec3 rhs) const;
};
