#include "Light.h"

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
