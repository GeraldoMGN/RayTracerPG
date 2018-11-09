#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"
#include "ObjectIntersection.h"

#include <float.h>
#include <iostream>

//consts
double const PI = 3.14159265359;

//ISSO DEVERIA SER LIDO DE UM ARQUIVO
int imageWidth = 400;
int imageHeight = 400;

void render(Image& image, Scene& scene, Camera& camera) {
	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			//camera fixa em 0,0,0 (necessita de uma matriz cameraToWorld)
			double aspectRatio = (double)imageWidth / (double)imageHeight;
			double Px = (2 * ((x + 0.5) / (double)imageWidth) - 1) * tan(camera.getFov() / 2 * PI / 180) * aspectRatio;
			double Py = (1 - 2 * ((y + 0.5) / (double)imageHeight) * tan(camera.getFov() / 2 * PI / 180));
			Vec3 origin(0.0f);
			Vec3 direction(Px, Py, -1);
			direction.normalize();
			Ray ray(origin, direction);

			double nearestIntersec = DBL_MAX;
			for (int k = 0; k < scene.getNumberObjects(); k++) {
				double intersec = DBL_MAX;
				//if it intersects and is the intersection is less than nearestIntersec
				ObjectIntersection* intersecInfo = new ObjectIntersection{ 0.0f, Vec3(0.0f), Vec3(0.0f), NULL };
				if (scene.getObject(k)->intersect(ray, intersecInfo))
					image.SetPixel(x, y, new Vec3(1.0f));
				else image.SetPixel(x, y, new Vec3(0.0f));
				nearestIntersec = intersec;
			}
		}
	}
	image.SaveAsPPM();
}

int main() {
	//inicializando a imagem
	Image image = Image(imageWidth, imageHeight);
	//inicializando objetos e cenas
	Sphere sphere1Geometry = Sphere(Vec3(10.0f, 10.0f, -50.0f), 10.0f);
	Material sphere1Material = Material(1.0f, 1.0f, 1.0f, 1.0f, Vec3(1.0f));
	Object sphere1 = Object(&sphere1Geometry, &sphere1Material);
	Scene scene;
	scene.add(&sphere1);
	Camera camera = Camera(Vec3(0.0f), Vec3(0.0f, 0.0f, 1.0f),
		Vec3(0.0f, 1.0f, 0.0f), 90, 1.0f);
	render(image, scene, camera);

	return 0;
}