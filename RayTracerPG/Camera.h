#pragma once

#include "Vec3.h"
#include "Ray.h"

#include <cmath>

//consts
double const PI = 3.14159265359;

class Camera {
private:
	Vec3 position, target, up;
	double fov, f;
public:
	Camera(Vec3 position, Vec3 target, Vec3 up, double fov, double f);
	//retorna um Ray que vai ser emitido por tal pixel
	Ray getRay(double x, double y, int width, int height) const;
	
	double getFov() const;
	Vec3 getPos() const;
};
