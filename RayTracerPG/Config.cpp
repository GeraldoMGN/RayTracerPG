#include "Config.h"

#include <fstream>
#include <iostream>
#include <sstream>

void Config::readConfigFile(Image* image)
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
			std::string temp;
			std::getline(is_line, temp, ' ');
			int w = stoi(temp);
			std::getline(is_line, temp);
			int h = stoi(temp);
			setRes(w, h, image);
		}
			
	}
}

void Config::setRes(int w, int h, Image* image)
{
	std::cout << "Criando uma imagem com " << w << " pixeis por " << h << " pixeis." << std::endl;
	*image = Image(w, h);

}
