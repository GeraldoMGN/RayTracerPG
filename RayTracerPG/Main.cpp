#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Geometry.h"

#include <float.h>


//ISSO DEVERIA SER LIDO DE UM ARQUIVO
int imageWidth = 400;
int ImageHeight = 400;



int main() {
	//inicializando a imagem
	Image image = Image(imageWidth, ImageHeight);
	//inicializando objetos e cenas
	Sphere sphere1Geometry = Sphere(Vec3(0.0f, 0.0f, 100.0f), 10.0f);
	Material sphere1Material = Material(1.0f, 1.0f, 1.0f, 1.0f, Vec3(1.0f, 1.0f, 1.0f));
	Object sphere1 = Object(&sphere1Geometry, &sphere1Material);
	Scene scene;
	scene.add(&sphere1);

	render(image, scene);

	return 0;
}

void render(Image& image, Scene& scene) {
	for (int i = 0; i < image.getHeight(); i++) {
		for (int j = 0; j < image.getWidth(); j++) {
			//TODO: primary ray

			double nearestIntersec = DBL_MAX;
			for (int k = 0; k < scene.getNumberObjects(); k++) {
				double intersec = DBL_MAX;
				//if it intersects and is the intersection is less than nearestIntersec
					nearestIntersec = intersec;
			}
		}
	}
}