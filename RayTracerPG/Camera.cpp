#include "Camera.h"

Camera::Camera(Vec3 position, Vec3 target, Vec3 up, double fov, double f) : position(position), target(target), up(up), fov(fov), f(f) {}

double Camera::getFov() const
{
	return this->fov;
}

Vec3 Camera::getPos() const
{
	return this->position;
}
