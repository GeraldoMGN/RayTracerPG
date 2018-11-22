#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"
#include "ObjectIntersection.h"
#include "Consts.h"
#include "Config.h"
#include "MaterialList.h"

#include <float.h>
#include <algorithm>
#include <iostream>

//Quando os vetores estão normalizados, retorna 1 se tiver paralelo a 0 caso esteja perpendicular.
double facingRatio(Vec3 vector1, Vec3 vector2) {
	return std::max(0.0, vector1.dotProduct(Vec3(0.0) - vector2));
}

//Pega o ObjectIntersection com as informações da interseção mais proxima
ObjectIntersection* castRay(Ray& ray, Scene* scene) {
	ObjectIntersection* intersecInfo = new ObjectIntersection{DBL_MAX, Vec3(0.0f), Vec3(0.0f), NULL};
	
	//compara com cada objeto da cena para ver se há ponto de interseção mais próximo que o anterior
	for (int k = 0; k < scene->getNumberObjects(); k++) {
		ObjectIntersection* tempInfo = new ObjectIntersection{0.0f, Vec3(0.0f), Vec3(0.0f), NULL};
		if (scene->getObject(k)->intersect(ray, tempInfo)) {
			if (tempInfo->t < intersecInfo->t) {
				intersecInfo = tempInfo;
			}
		}
	}
	if (intersecInfo->t == DBL_MAX)
		return NULL;
	return intersecInfo;
}

//calcula a cor do pixel
Vec3* shade(Ray& ray, ObjectIntersection* intersecInfo, Scene* scene) {
	if (intersecInfo == NULL)
		return new Vec3(0.5f);

	Vec3 color = Vec3(0.0f);
	Vec3 difuse = Vec3(0.0f);
	Vec3 specular = Vec3(0.0f);
	Vec3 materialColor = intersecInfo->o->getMaterial()->getColor();

	//loop para todas as luzes
	for (int i = 0; i < scene->getNumberLights(); i++) {
		Vec3 lightDirection = (intersecInfo->p - scene->getLight(i)->getPosition()).normalize();
		Vec3 lightIntensity = scene->getLight(i)->intensityAtP(intersecInfo->p);

		Ray shadowRay = Ray(intersecInfo->p, Vec3(0.0f) - lightDirection);
		ObjectIntersection* shadowIntersec = castRay(shadowRay, scene);
		
		bool directLight = (shadowIntersec == NULL || shadowIntersec->t >= (intersecInfo->p - scene->getLight(i)->getPosition()).length());

		if (directLight) {
			difuse = difuse + (materialColor * lightIntensity * facingRatio(intersecInfo->n, lightDirection));

			Vec3 reflected = lightDirection.reflect(intersecInfo->n);
			specular = specular + (lightIntensity * std::pow(facingRatio(reflected, lightDirection), intersecInfo->o->getMaterial()->getAlpha()));
		}
	}

	color = difuse * intersecInfo->o->getMaterial()->getKd() + 
		(specular * intersecInfo->o->getMaterial()->getKs()) +
		materialColor * intersecInfo->o->getMaterial()->getKe();
	return new Vec3(color);
}

void render(Image* image, Scene* scene, Camera* camera) {
	for (int y = 0; y < image->getHeight(); y++) {
		for (int x = 0; x < image->getWidth(); x++) {
			Ray ray = camera->getRay(x, y, image->getWidth(), image->getHeight());
			ObjectIntersection* intersecInfo = castRay(ray, scene);
			Vec3* pixelColor = shade(ray, intersecInfo, scene);
			image->SetPixel(x, y, pixelColor);
		}
	}
	image->SaveAsPPM();
}

int main() {
	Image* image = new Image(0, 0);
	Camera* camera = new Camera(Vec3(0),Vec3(0), Vec3(0), 0, 0);
	MaterialList* materialList = new MaterialList();
	Scene* scene = new Scene();

	Config::readConfigFile(image, camera, scene, materialList);
	camera->setCamToWorldMatrix();

	//luzes
	Light light = Light(Vec3(0.0f, 30.0f, 20.0f), Vec3(1.0f, 0.0f, 0.0f), 20000.0f);
	scene->addLight(&light);
	Light light2 = Light(Vec3(-10.0f, 30.0f, 20.0f), Vec3(0.0f, 1.0f, 0.0f), 20000.0f);
	scene->addLight(&light2);
	Light light3 = Light(Vec3(10.0f, 30.0f, 20.0f), Vec3(0.0f, 0.0f, 1.0f), 20000.0f);
	scene->addLight(&light3);
	//camera
	render(image, scene, camera);
	std::cout << "Aperte enter para sair:";
	std::cin.get();

	return 0;
}