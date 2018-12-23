#pragma once

#include "Vec3.h"

#include <String>
#include <vector>

class Material {
private:
	Vec3 color; //color
	double Ka, Kd, Ks, alpha; //Fator Emissivo, Difuso e Especular, Coeficiente de Rugosidade
	std::string name;
public:
	Material(std::string name, double Ke, double Kd, double Ks, double alpha, Vec3 c);

	double getKa() const;
	double getKd() const;
	double getKs() const;
	double getAlpha() const;
	Vec3 getColor() const;
	std::string getName() const;
};