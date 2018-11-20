#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Config::readConfigFile(Image* image, Camera* camera)
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
	}
}

void Config::setRes(std::istringstream& line, Image* image)
{
	std::string temp;
	std::getline(line, temp, ' ');
	int w = stoi(temp);
	std::getline(line, temp);
	int h = stoi(temp);
	std::cout << "Criando uma imagem com " << w << " pixeis por " << h << " pixeis." << std::endl;
	*image = Image(w, h);

}

void Config::setCamera(std::istringstream& line, Camera* camera)
{
	std::string temp;
	std::getline(line, temp, ' ');
	double px = stod(temp);
	std::getline(line, temp, ' ');
	double py = stod(temp);
	std::getline(line, temp, ' ');
	double pz = stod(temp);
	std::getline(line, temp, ' ');
	double tx = stod(temp);
	std::getline(line, temp, ' ');
	double ty = stod(temp);
	std::getline(line, temp, ' ');
	double tz = stod(temp);
	std::getline(line, temp, ' ');
	double ux = stod(temp);
	std::getline(line, temp, ' ');
	double uy = stod(temp);
	std::getline(line, temp, ' ');
	double uz = stod(temp);
	std::getline(line, temp, ' ');
	double fov = stod(temp);
	std::getline(line, temp);
	double f = stod(temp);
	std::cout << "Criando uma camera em: X = " << px << " Y = " << py << " Z = " << pz << std::endl <<
				 "       apontando para: X = " << tx << " Y = " << ty << " Z = " << tz << std::endl <<
				 "  vetor de orientacao: X = " << ux << " Y = " << uy << " Z = " << uz << std::endl <<
		         "com " << fov << " graus de campo de visao e " << f << " de distancia para o plano da imagem.";
	*camera = Camera(Vec3(px, py, pz), Vec3(tx, ty, tz), Vec3(ux, uy, uz), fov, f);
}
