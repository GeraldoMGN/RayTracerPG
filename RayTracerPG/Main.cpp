#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"
#include "ObjectIntersection.h"

#include <float.h>
#include <algorithm>
#include <iostream>

//consts

//ISSO DEVERIA SER LIDO DE UM ARQUIVO
int imageWidth = 1920;
int imageHeight = 1080;

//calcula a cor do pixel
Vec3* shade(Ray& ray, ObjectIntersection* intersecInfo) {
	if (intersecInfo == NULL)
		return new Vec3(0.5f);
	
	//Já que os vetores estão normalizados, retorna 1 se tiver paralelo a 0 caso esteja perpendicular.
	double facingRation = std::max(0.0, intersecInfo->n.dotProduct(Vec3(0.0) - ray.getDirection()));
	//retorno temporário
	return new Vec3(facingRation);
}

//Pega o ObjectIntersection com as informações da interseção mais proxima
ObjectIntersection* castRay(Ray& ray, Scene& scene) {
	ObjectIntersection* intersecInfo = new ObjectIntersection{DBL_MAX, Vec3(0.0f), Vec3(0.0f), NULL};
	
	//compara com cada objeto da cena para ver se há ponto de interseção mais próximo que o anterior
	for (int k = 0; k < scene.getNumberObjects(); k++) {
		ObjectIntersection* tempInfo = new ObjectIntersection{0.0f, Vec3(0.0f), Vec3(0.0f), NULL};
		if (scene.getObject(k)->intersect(ray, tempInfo)) {
			if (tempInfo->t < intersecInfo->t) {
				intersecInfo = tempInfo;
			}
		}
	}
	if (intersecInfo->t == DBL_MAX)
		return NULL;
	return intersecInfo;
}

void render(Image& image, Scene& scene, Camera& camera) {
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			Ray ray = camera.getRay(x, y, image.getWidth(), image.getHeight());
			ObjectIntersection* intersecInfo = castRay(ray, scene);
			Vec3* pixelColor = shade(ray, intersecInfo);
			image.SetPixel(x, y, pixelColor);
		}
	}
	image.SaveAsPPM();
}

int main() {
	//inicializando a imagem
	Image image = Image(imageWidth, imageHeight);
	//inicializando objetos e cenas
	Sphere sphere1Geometry = Sphere(Vec3(10.0f, 10.0f, 30.0f), 10.0f);
	Material sphere1Material = Material(1.0f, 1.0f, 1.0f, 1.0f, Vec3(1.0f));
	Object sphere1 = Object(&sphere1Geometry, &sphere1Material);
	Sphere sphere2Geometry = Sphere(Vec3(-10.0f, -10.0f, 40.0f), 10.0f);
	Object sphere2 = Object(&sphere2Geometry, &sphere1Material);
	Scene scene;
	scene.add(&sphere1);
	scene.add(&sphere2);
	Camera camera = Camera(Vec3(0.0f), Vec3(0.0f, 0.0f, 1.0f),
		Vec3(0.0f, 1.0f, 0.0f), 90, 1.0f);
	camera.setCamToWorldMatrix();
	render(image, scene, camera);

	return 0;
}