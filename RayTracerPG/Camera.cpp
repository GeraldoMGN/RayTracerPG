#include "Camera.h"

Camera::Camera(Vec3 position, Vec3 target, Vec3 up, double fov, double f) : position(position), target(target), up(up), fov(fov), f(f) {}

Ray Camera::getRay(double x, double y, int width, int height) const
{
	//camera fixa em 0,0,0 (necessita de uma matriz cameraToWorld)
	double aspectRatio = (double)width / (double)height;
	double Px = (2 * ((x + 0.5) / (double)width) - 1) * tan(this->fov / 2 * PI / 180) * aspectRatio;
	double Py = (1 - 2 * ((y + 0.5) / (double)height) * tan(this->fov / 2 * PI / 180));
	Vec3 origin = this->position;
	Vec3 direction(Px, Py, -1);
	direction.normalize();
	return Ray(origin, direction);
}

double Camera::getFov() const
{
	return this->fov;
}

Vec3 Camera::getPos() const
{
	return this->position;
}
