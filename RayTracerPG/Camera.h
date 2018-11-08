#pragma once

#include "Vec3.h"
#include "Ray.h"

#include <cmath>

class Camera {
private:
	Vec3 position, target, up;
	double fov, f;
public:
	Camera(Vec3 position, Vec3 target, Vec3 up, double fov, double f);
	Ray GetRay(double x, double y, int width, int height) const;
	double getFov() const;
};
