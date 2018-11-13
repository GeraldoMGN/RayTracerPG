#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"
#include "ObjectIntersection.h"

#include <float.h>
#include <iostream>

//consts

//ISSO DEVERIA SER LIDO DE UM ARQUIVO
int imageWidth = 1280;
int imageHeight = 720;

//calcula a cor do pixel
Vec3* shade(ObjectIntersection* intersecInfo) {
	//método temporário
	if (intersecInfo == NULL)
		return new Vec3(0.0f);
	return new Vec3(1.0f);
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
			Vec3* pixelColor = shade(intersecInfo);
			image.SetPixel(x, y, pixelColor);
		}
	}
	image.SaveAsPPM();
}

int main() {
	//inicializando a imagem
	Image image = Image(imageWidth, imageHeight);
	//inicializando objetos e cenas
	Sphere sphere1Geometry = Sphere(Vec3(10.0f, 10.0f, 50.0f), 10.0f);
	Material sphere1Material = Material(1.0f, 1.0f, 1.0f, 1.0f, Vec3(1.0f));
	Object sphere1 = Object(&sphere1Geometry, &sphere1Material);
	Sphere sphere2Geometry = Sphere(Vec3(-10.0f, -10.0f, 50.0f), 10.0f);
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