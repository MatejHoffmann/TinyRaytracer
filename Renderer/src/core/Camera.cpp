#include "Camera.h"

#include <string>

#include "Glar.h"
#include "../utils/Sampler.h"

glar::Camera::Camera(const int width, const int height, const Point& p, const Point& lookAt, const double fov)
	: width(width),
	height(height),
	position(p),
	lookAt(lookAt),
	fov(fov)
{
	nPixels = width * height;
	aspect = (double)width / (double)height;
	double theta = (fov * PI) / 180;
	double viewHalfHeight = std::tan(theta / 2);
	double viewHalfWidth = viewHalfHeight * aspect;

	u = (position - lookAt).normalize();
	n = cross(Vector(0, 1, 0), u).normalize();
	v = cross(u, n).normalize();

	Point center = position - u;

	pixelSizeX = (viewHalfWidth * 2.0) / width;
	pixelSizeY = (viewHalfHeight * 2.0) / height;
	
	upperCorner = center - n * viewHalfWidth + v * viewHalfHeight;
}

glar::Ray glar::Camera::generateRay(int i, int j)
{
	Sampler sampler;
	double x = pixelSizeX * ((double)i + sampler.uniformSampleOne());
	double y = pixelSizeY * ((double)j + sampler.uniformSampleOne());

	Point pixelPoint = upperCorner + n * x  + (-v) * y;
	return Ray(position, pixelPoint);
}

glar::Ray glar::Camera::generateRay(int index)
{
	if (index >= nPixels)
		throw std::invalid_argument("Argument value is invalid, must be in range (0, " + std::to_string(nPixels - 1) + ")");
	
	int i = index % width;
	int j = index / width;

	Sampler sampler;
	double x = pixelSizeX * ((double)i + sampler.uniformSampleOne());
	double y = pixelSizeY * ((double)j + sampler.uniformSampleOne());

	Point pixelPoint = upperCorner + n * x + (-v) * y;
	return Ray(position, pixelPoint);
}

int glar::Camera::getHeight() const
{
	return height;
}

int glar::Camera::getWidth() const
{
	return width;
}
