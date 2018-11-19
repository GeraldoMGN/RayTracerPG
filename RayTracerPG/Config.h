#pragma once

#include "Image.h"

#include <string>

class Config {
public:
	static void readConfigFile(Image* image);
	static void setRes(int w, int h, Image* image);
	static void setCamera(double px, double py, double pz,
		double tx, double ty, double tz,
		double ux, double uy, double uz,
		double fov, double f);
	static void createMaterial(std::string name, double r, double g, double b,
		double Kd, double Ks, double Ke, double alpha);
	static void createSphere(double cx, double cy, double cz,
		double r, std::string materialName);
	static void createMesh(std::string fileName, std::string materialName);
	static void createLight(double px, double py, double pz
		, double r, double g, double b, double intensity);
};