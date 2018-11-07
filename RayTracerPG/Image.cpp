#include "Image.h"
#include <iostream>
#include <fstream>
#include <algorithm>

Image::Image(int width, int height) : width(width), height(height) {
	buffer = new Vec3*[width * height];
	for (int i = 0; i < width * height; i++) {
		buffer[i] = new Vec3(1.0f, 0.0f, 1.0f);
	}
}

void Image::SetPixel(int x, int y, Vec3* color) {
	buffer[x + (y * this->width)] = color;
}

void Image::SaveAsPPM() const {
	std::ofstream ofs("./image.ppm", std::ios::out | std::ios::binary);
	ofs << "P3\n" << width << " " << height << "\n255\n";
	for (int i = 0; i < width * height; i++) {
		ofs << (unsigned int)(std::min(double(1), buffer[i]->getX()) * 255) << " " <<
			(unsigned int)(std::min(double(1), buffer[i]->getY()) * 255) << " " <<
			(unsigned int)(std::min(double(1), buffer[i]->getZ()) * 255) << " ";
	}
	ofs.close();
}

int Image::getWidth() const
{
	return this->width;
}

int Image::getHeight() const
{
	return this->height;
}
