#pragma once

#include "Vec3.h"
#include "Consts.h"

class Light {
private:
	Vec3 position, color;
	double intensity;
public:
	Light(Vec3 position, Vec3 color, double intensity);
	//retorna a intensidade da luz no ponto P (RGB)
	Vec3 intensityAtP(Vec3& P) const;
	
	Vec3 getPosition() const;
	Vec3 getColor() const;
	double getIntensity() const;
};