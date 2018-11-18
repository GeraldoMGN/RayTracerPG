#include "Light.h"

Light::Light(Vec3 position, Vec3 color, double intensity) : position(position), color(color), intensity(intensity) {}

Vec3 Light::intensityAtP(Vec3& P) const
{
	double distance = (P - position).length();
	return (color / (4 * PI * distance * distance)) * intensity;
}

Vec3 Light::getPosition() const
{
	return this->position;
}

Vec3 Light::getColor() const
{
	return this->color;
}

double Light::getIntensity() const
{
	return this->intensity;
}
