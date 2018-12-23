#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"
#include "ObjectIntersection.h"
#include "Consts.h"
#include "Config.h"
#include "MaterialList.h"

#define TINYOBJLOADER_USE_DOUBLE
#define TINYOBJLOADER_IMPLEMENTATION
#include "Include/tiny_obj_loader.h"

#include <thread>
#include <vector>
#include <float.h>
#include <algorithm>
#include <iostream>
#include <chrono>

std::vector<std::thread> threads;

//Quando os vetores estão normalizados, retorna 1 se tiver paralelo a 0 caso esteja perpendicular.
double facingRatio(Vec3 vector1, Vec3 vector2) {
	return std::max(0.0, vector1.dotProduct(Vec3(0.0) - vector2));
}

//Pega o ObjectIntersection com as informações da interseção mais proxima
ObjectIntersection* castRay(Ray& ray, Scene* scene) {
	ObjectIntersection* intersecInfo = new ObjectIntersection{DBL_MAX, Vec3(0.0f), Vec3(0.0f), NULL};
	
	//compara com cada objeto da cena para ver se há ponto de interseção mais próximo que o anterior
	for (int k = 0; k < scene->getNumberObjects(); k++) {
		ObjectIntersection* tempInfo = new ObjectIntersection{ DBL_MAX, Vec3(0.0f), Vec3(0.0f), NULL};
		if (scene->getObject(k)->intersect(ray, tempInfo)) {
			if (tempInfo->t < intersecInfo->t) {
				delete[] intersecInfo;
				intersecInfo = tempInfo;
			}
		}
	}
	if (intersecInfo->t == DBL_MAX)		//caso não haja interseção
		return NULL;
	return intersecInfo;
}

//retorna a cor do backgound caso não haja interseção
Vec3* backgroundColor(const int& x, const int& y, const int& maxX, const int& maxY) {
	double maxDistance = sqrt(((maxX / 2) * (maxX / 2)) + (maxY/2 * maxY/2));
	double xDistance = abs(x - maxX/2);
	double yDistance = abs(y - maxY/2);
	double distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
	double gradient = distance / maxDistance;
	Vec3 color = Vec3(0.3) * gradient + Vec3(0.6) * (1.0 - gradient);
	return new Vec3(color);
}

//calcula a cor do pixel
Vec3* shade(Ray& ray, ObjectIntersection* intersecInfo, Scene* scene) {
	if (intersecInfo == NULL)
		return NULL;

	Vec3 color = Vec3(0.0f);
	Vec3 difuse = Vec3(0.0f);
	Vec3 specular = Vec3(0.0f);
	Vec3 materialColor = intersecInfo->o->getMaterial()->getColor();

	//loop para todas as luzes
	for (int i = 0; i < scene->getNumberLights(); i++) {
		Vec3 lightDirection = (intersecInfo->p - scene->getLight(i)->getPosition()).normalize();
		Vec3 lightIntensity = scene->getLight(i)->intensityAtP(intersecInfo->p);
		Vec3 hitPoint = intersecInfo->p + (Vec3(0.0f) - lightDirection) * BIAS;		
		//Bias necessario para resolver problemas de precisão de ponto flutuante
		
		//raio de sombra
		Ray shadowRay = Ray(hitPoint, Vec3(0.0f) - lightDirection);
		ObjectIntersection* shadowIntersec = castRay(shadowRay, scene);
		
		bool directLight = (shadowIntersec == NULL || shadowIntersec->t >= (intersecInfo->p - scene->getLight(i)->getPosition()).length());

		if (directLight) {
			//componente difuso para uma luz
			difuse = difuse + (materialColor * lightIntensity * facingRatio(intersecInfo->n, lightDirection));
			//componente especular para uma luz
			Vec3 reflected = lightDirection.reflect(intersecInfo->n);
			specular = specular + (lightIntensity * std::pow(facingRatio(reflected, lightDirection), intersecInfo->o->getMaterial()->getAlpha()));
		}
		delete[] shadowIntersec;
	}
	
	//adição dos diversos componentes (difuso, especular e emissivo)
	color = difuse    * intersecInfo->o->getMaterial()->getKd() + 
		specular      * intersecInfo->o->getMaterial()->getKs() +
		materialColor * intersecInfo->o->getMaterial()->getKa();
	return new Vec3(color);
}

void renderLine(int y, Image* image, Scene* scene, Camera* camera) {
	for (int x = 0; x < image->getWidth(); x++) {
		//criação do raio
		Ray ray = camera->getRay(x, y, image->getWidth(), image->getHeight());
		//Informaçoes da interseção com o objeto mais próximo
		ObjectIntersection* intersecInfo = castRay(ray, scene);
		//cor do pixel
		Vec3* pixelColor = shade(ray, intersecInfo, scene);
		if (pixelColor == NULL)
			pixelColor = backgroundColor(x, y, image->getWidth(), image->getHeight());
		image->SetPixel(x, y, pixelColor);

		if (x == 0 && (y + 1) % 100 == 0)
			std::cout << "line " << y + 1 << std::endl;
		delete[] intersecInfo;
	}
}

//Separa o processamento por linha
void render(Image* image, Scene* scene, Camera* camera) {
	for (int y = 0; y < image->getHeight(); y++) {
		threads.push_back(std::thread(renderLine, y, image, scene, camera));
	}
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main() {
	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
	//Pointers
	Image* image = new Image(0, 0);
	Camera* camera = new Camera(Vec3(0),Vec3(0), Vec3(0), 0, 0);
	MaterialList* materialList = new MaterialList();
	Scene* scene = new Scene();

	Config::readConfigFile(image, camera, scene, materialList);
	std::cout << "Config file read" << std::endl;
	camera->setCamToWorldMatrix();

	std::cout << "Rendering..." << std::endl;
	startTime = std::chrono::system_clock::now();
	render(image, scene, camera);
	endTime = std::chrono::system_clock::now();
	
	std::cout << "Rendering complete. Saving file." << std::endl;
	image->SaveAsPPM();
	
	std::chrono::duration<double> elapsedTime = endTime - startTime;
	std::cout << "Render time: " <<	elapsedTime.count() << " seconds" << std::endl;
	std::cout << "Press any key to close";
	std::cin.get();

	return 0;
}