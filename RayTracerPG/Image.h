#pragma once

#include "Vec3.h"

#include <vector>

class Image {
private:
	int width, height;
	Vec3** buffer;
public:
	Image(int width, int height);
	void SetPixel(int x, int y, Vec3* color);
	void SaveAsPPM() const;
	int getWidth() const;
	int getHeight() const;
};
