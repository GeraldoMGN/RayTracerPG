#include "Material.h"

Material::Material(std::string name, double Ke, double Kd, double Ks, double alpha, Vec3 c)
	:name(name), Ke(Ke), Kd(Kd), Ks(Ks), alpha(alpha), c(c)
{}

double Material::getKe() const
{
	return this->Ke;
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
	return this->c;
}

std::string Material::getName() const
{
	return this->name;
}
