#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Config::readConfigFile(Image* image, Camera* camera, Scene* scene, MaterialList* materialList)
{
	std::ifstream file("./config.txt");
	std::string line;
	while (std::getline(file, line)) {		//para cada linha
		if (line[0] == '#')					//comentário
			continue;
		if (line.empty())					//linha em branco
			continue;
		
		std::string key;
		std::istringstream is_line(line);
		
		std::getline(is_line, key, ' ');
		if (key == "res") {						//resoluçao
			setRes(is_line, image);
		}
		if (key == "camera") {					//camera
			setCamera(is_line, camera);
		}
		if (key == "material") {					//camera
			createMaterial(is_line, materialList);
		}
		if (key == "sphere") {					//camera
			createSphere(is_line, scene, materialList);
		}
	}
}

void Config::setRes(std::istringstream& line, Image* image)
{
	int w = readInt(line);
	int h = readInt(line);
	
	std::cout << "Criando uma imagem com " << w << " pixeis por " << h << " pixeis." << std::endl << std::endl;
	
	*image = Image(w, h);
}

void Config::setCamera(std::istringstream& line, Camera* camera)
{
	double px  = readDouble(line);
	double py  = readDouble(line);
	double pz  = readDouble(line);
	double tx  = readDouble(line);
	double ty  = readDouble(line);
	double tz  = readDouble(line);
	double ux  = readDouble(line);
	double uy  = readDouble(line);
	double uz  = readDouble(line);
	double fov = readDouble(line);
	double f   = readDouble(line);
	
	std::cout << "Criando uma camera em: X = " << px << " Y = " << py << " Z = " << pz << std::endl <<
		"       apontando para: X = " << tx << " Y = " << ty << " Z = " << tz << std::endl <<
		"  vetor de orientacao: X = " << ux << " Y = " << uy << " Z = " << uz << std::endl <<
		"com " << fov << " graus de campo de visao e " << f << " de distancia para o plano da imagem." << std::endl << std::endl;
	
	*camera = Camera(Vec3(px, py, pz), Vec3(tx, ty, tz), Vec3(ux, uy, uz), fov, f);
}

void Config::createMaterial(std::istringstream& line, MaterialList* materialList)
{
	std::string name = readString(line);
	double r = readDouble(line);
	double g = readDouble(line);
	double b = readDouble(line);
	double Kd = readDouble(line);
	double Ks = readDouble(line);
	double Ke = readDouble(line);
	double alpha = readDouble(line);

	Material* material = new Material(name, Ke, Kd, Ks, alpha, Vec3(r, g, b));
	if (!materialList->addMaterial(material)){
		std::cout << "Nome " << name << " ja existente." << std::endl << std::endl;
	} else {
		std::cout << "Criando " + name + " de cor: R = " << r << " G = " << g << " B = " << b << std::endl <<
			"Coeficientes: Kd = " << Kd << " Ks = " << Ks << " Ke = " << Ke << " Alpha = " << alpha << std::endl << std::endl;
	}
}

void Config::createSphere(std::istringstream& line, Scene* scene, MaterialList* materialList)
{
	double cx				 = readDouble(line);
	double cy				 = readDouble(line);
	double cz			     = readDouble(line);
	double r				 = readDouble(line);
	std::string materialName = readString(line);

	Material* material = materialList->getMaterial(materialName);
	Sphere* sphereGeometry = new Sphere(Vec3(cx, cy, cz), r);
	Object* sphere = new Object(sphereGeometry, material);
	scene->addObject(sphere);
}

int Config::readInt(std::istringstream& line)
{
	std::string temp;
	std::getline(line, temp, ' ');
	return stoi(temp);
}

double Config::readDouble(std::istringstream& line)
{
	std::string temp;
	std::getline(line, temp, ' ');
	return stod(temp);
}

std::string Config::readString(std::istringstream& line)
{
	std::string temp;
	std::getline(line, temp, ' ');
	return temp;
}
