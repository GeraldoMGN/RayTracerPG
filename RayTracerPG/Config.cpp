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
		else if (key == "camera") {					//camera
			setCamera(is_line, camera);
		}
		else if (key == "material") {					//camera
			createMaterial(is_line, materialList);
		}
		else if (key == "sphere") {					//camera
			createSphere(is_line, scene, materialList);
		}
		else if (key == "mesh") {					//camera
			createMesh(is_line, scene, materialList);
		}
		else if (key == "light") {					//camera
			createLight(is_line, scene);
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
	if (material == NULL) {
		std::cout << "Material não encontrado, esfera não criada" << std::endl << std::endl;
		return;
	}
	Sphere* sphereGeometry = new Sphere(Vec3(cx, cy, cz), r);
	Object* sphere = new Object(sphereGeometry, material);

	std::cout << "Criando uma esfera em: X = " << cx << " Y = " << cy << " Z = " << cz << std::endl <<
		"com raio = " << r << ", usando o material: " << material->getName() << std::endl << std::endl;

	scene->addObject(sphere);
}

void Config::createMesh(std::istringstream& line, Scene* scene, MaterialList* materialList)
{
	std::string fileName = readString(line);
	std::string materialName = readString(line);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	std::string path = "objs/" + fileName;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
		std::cout << err << std::endl;
		throw std::runtime_error(err);
	}
	Mesh* mesh = new Mesh(attrib.vertices, shapes[0].mesh.indices, attrib.normals, shapes[0].mesh.num_face_vertices);
	Material* material = materialList->getMaterial(materialName);
	Object* object = new Object(mesh, material);
	scene->addObject(object);
	std::cout << "Mesh " << fileName << " Carregada, usando o material: " << material->getName() << std::endl << std::endl;
}

void Config::createLight(std::istringstream& line, Scene* scene)
{
	double px = readDouble(line);
	double py = readDouble(line);
	double pz = readDouble(line);
	double r = readDouble(line);
	double g = readDouble(line);
	double b = readDouble(line);
	double intensity = readDouble(line);

	std::cout << "Criando uma luz pontual em: X = " << px << " Y = " << pz << " Z = " << pz << std::endl <<
		" de cor: R = " << r << " G = " << g << " B = " << b << " e intensidade = " << intensity << std::endl << std::endl;

	Light* light = new Light(Vec3(px, py, pz), Vec3(r, g, b), intensity);
	scene->addLight(light);
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
