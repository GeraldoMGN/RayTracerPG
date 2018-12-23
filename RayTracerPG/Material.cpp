#include "Material.h"

Material::Material(std::string name, double Ka, double Kd, double Ks, double alpha, Vec3 color)
	:name(name), Ka(Ka), Kd(Kd), Ks(Ks), alpha(alpha), color(color)
{}

double Material::getKa() const
{
	return this->Ka;
}

double Material::getKd() const
{
	return this->Kd;
}

double Material::getKs() const
{
	return this->Ks;
}

double Material::getAlpha() const
{
	return this->alpha;
}

Vec3 Material::getColor() const
{
	return this->color;
}

std::string Material::getName() const
{
	return this->name;
}
