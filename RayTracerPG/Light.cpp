#include "Light.h"

Light::Light(Vec3 position, Vec3 color, double intensity) : position(position), color(color), intensity(intensity) {}

Vec3 Light::intensityAtP(Vec3& P) const
{
	//A intensidade da luz em um ponto P é calculada em função da área da superficie da esfera
	double distance = (P - position).length();
	return (color  * intensity) / 
		(4 * PI * distance * distance);
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
