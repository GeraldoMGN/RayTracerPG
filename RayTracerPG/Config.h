#pragma once

#include "Image.h"
#include "Camera.h"

#include <string>

class Config {
public:
	static void readConfigFile(Image* image, Camera* camera);
	static void setRes(std::istringstream& line, Image* image);
	static void setCamera(std::istringstream& line, Camera* camera);
	static void createMaterial(std::string name, double r, double g, double b,
		double Kd, double Ks, double Ke, double alpha);
	static void createSphere(double cx, double cy, double cz,
		double r, std::string materialName);
	static void createMesh(std::string fileName, std::string materialName);
	static void createLight(double px, double py, double pz
		, double r, double g, double b, double intensity);
};