#include "Camera.h"

#include <iostream>


Camera::Camera(Vec3 position, Vec3 target, Vec3 up, double fov, double f) : position(position), target(target), up(up), fov(fov), f(f) {}

Ray Camera::getRay(double x, double y, int width, int height) const
{
	//TODO: implementar distancia do plano de imagem
	double aspectRatio = (double)width / (double)height;
	//std::cout << "Aspect Ratio: " << aspectRatio << std::endl;
	double Px = (2 * ((x + 0.5) / (double)width) - 1) * tan(this->fov / 2 * PI / 180) * aspectRatio;
	double Py = (1 - 2 * ((y + 0.5) / (double)height) * tan(this->fov / 2 * PI / 180));
	
	Vec3 origin = pointCamToWorld(Vec3(0.0f));
	Vec3 direction(Px, Py, -1);
	direction = vectorCamToWorld(direction);
	direction = direction.normalize();
	
	return Ray(origin, direction);
}

void Camera::setCamToWorldMatrix()
{
	Vec3 forward((position - target).normalize());
	Vec3 up = this->up.normalize();
	Vec3 right(up.crossProduct(forward));

	camToworldMatrix[0][0] = right.getX();
	camToworldMatrix[0][1] = right.getY();
	camToworldMatrix[0][2] = right.getZ();
	camToworldMatrix[0][3] = 0.0f;
	camToworldMatrix[1][0] = up.getX();
	camToworldMatrix[1][1] = up.getY();
	camToworldMatrix[1][2] = up.getZ();
	camToworldMatrix[1][3] = 0.0f;
	camToworldMatrix[2][0] = forward.getX();
	camToworldMatrix[2][1] = forward.getY();
	camToworldMatrix[2][2] = forward.getZ();
	camToworldMatrix[2][3] = 0.0f;
	camToworldMatrix[3][0] = this->position.getX();
	camToworldMatrix[3][1] = this->position.getY();
	camToworldMatrix[3][2] = this->position.getZ();
	camToworldMatrix[3][3] = 1.0f;

}

Vec3 Camera::pointCamToWorld(const Vec3& vector) const
{
	double x = vector.getX() * camToworldMatrix[0][0] + vector.getY() * camToworldMatrix[1][0] + vector.getZ() * camToworldMatrix[2][0] + camToworldMatrix[3][0];
	double y = vector.getX() * camToworldMatrix[0][1] + vector.getY() * camToworldMatrix[1][1] + vector.getZ() * camToworldMatrix[2][1] + camToworldMatrix[3][1];
	double z = vector.getX() * camToworldMatrix[0][2] + vector.getY() * camToworldMatrix[1][2] + vector.getZ() * camToworldMatrix[2][2] + camToworldMatrix[3][2];
	double w = vector.getX() * camToworldMatrix[0][3] + vector.getY() * camToworldMatrix[1][3] + vector.getZ() * camToworldMatrix[2][3] + camToworldMatrix[3][3];

	x = x / w;
	y = y / w;
	z = z / w;
	return Vec3(x, y, z);
}

Vec3 Camera::vectorCamToWorld(const Vec3& vector) const
{
	double x = vector.getX() * camToworldMatrix[0][0] + vector.getY() * camToworldMatrix[1][0] + vector.getZ() * camToworldMatrix[2][0];
	double y = vector.getX() * camToworldMatrix[0][1] + vector.getY() * camToworldMatrix[1][1] + vector.getZ() * camToworldMatrix[2][1];
	double z = vector.getX() * camToworldMatrix[0][2] + vector.getY() * camToworldMatrix[1][2] + vector.getZ() * camToworldMatrix[2][2];

	return Vec3(x, y, z);
}


double Camera::getFov() const
{
	return this->fov;
}

Vec3 Camera::getPos() const
{
	return this->position;
}
