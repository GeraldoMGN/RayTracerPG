#include "Material.h"

Material::Material(double Ke, double Kd, double Ks, double alpha, Vec3 c)
	:Ke(Ke), Kd(Kd), Ks(Ks), alpha(alpha), c(c)
{}

double Material::geKe() const
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
