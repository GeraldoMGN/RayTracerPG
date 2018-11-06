#pragma once

#include "Vec3.h"

class Material {
public:
	Material(double Ke, double Kd, double Ks, double alpha, Vec3 c);
private:
	Vec3 c; //cor
	double Ke, Kd, Ks, alpha; //Fator Emissivo, Difuso e Especular, Coeficiente de Rugosidade
};
