#pragma once

#include "Image.h"
#include "Camera.h"
#include "Scene.h"
#include "MaterialList.h"

#include <string>

class Config {
public:
	static void readConfigFile(Image* image, Camera* camera, Scene* scene, MaterialList* materialList);
	
	static void setRes (std::istringstream& line, Image* image);
	static void setCamera (std::istringstream& line, Camera* camera);
	static void createMaterial (std::istringstream& line, MaterialList* materialList);
	static void createSphere (std::istringstream& line, Scene* scene, MaterialList* materialList);
	static void createMesh (std::istringstream& line, Scene* scene, MaterialList* materialList);
	static void createLight (std::istringstream& line, Scene* scene);
	
	static int readInt (std::istringstream& line);
	static double readDouble (std::istringstream& line);
	static std::string readString(std::istringstream& line);
};