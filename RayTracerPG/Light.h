#pragma once

#include "Vec3.h"

class Light {
private:
	Vec3 position, color;
	double intensity;
public:
	Light(Vec3 position, Vec3 color, double intensity);
	Vec3 getPosition() const;
	Vec3 getColor() const;
	double getIntensity() const;
};