#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"
#include "ObjectIntersection.h"

#include <float.h>
#include <iostream>

//consts

//ISSO DEVERIA SER LIDO DE UM ARQUIVO
int imageWidth = 720;
int imageHeight = 720;

void render(Image& image, Scene& scene, Camera& camera) {
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			Ray ray = camera.getRay(x, y, image.getHeight(), image.getWidth());
			
			double nearestIntersec = DBL_MAX;
			for (int k = 0; k < scene.getNumberObjects(); k++) {
				double intersec = DBL_MAX;
				//if it intersects and is the intersection is less than nearestIntersec
				ObjectIntersection* intersecInfo = new ObjectIntersection{ 0.0f, Vec3(0.0f), Vec3(0.0f), NULL };
				if (scene.getObject(k)->intersect(ray, intersecInfo)) {
					if (intersecInfo->t < nearestIntersec) {
						image.SetPixel(x, y, new Vec3(1.0f));
						nearestIntersec = intersecInfo->t;
					}
				}
			}
			if (nearestIntersec == DBL_MAX)
				image.SetPixel(x, y, new Vec3(0.0f));
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