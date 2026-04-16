#include "Image.h"
#include <fstream>
#include <iostream>
#include <string>

glar::Image::Image(const int width, const int height)
	: width(width),
	height(height)
{
	nPixels = width * height;
	m_image = std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));
}

void glar::Image::saveImage(const std::string& path) const
{
	std::ofstream ofs;
	ofs.open(path);
	ofs << "P3\n" << width << " " << height << "\n255\n";
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			glar::Color c = m_image[y][x].color / m_image[y][x].nSamples;
			c.clamp();
			int r = (c.r) * 255;
			int g = (c.g) * 255;
			int b = (c.b) * 255;
			ofs << r << " " << g << " " << b << " ";
		}
	}
}

glar::Pixel glar::Image::getPixel(int x, int y) const
{
	return m_image[y][x];
}

void glar::Image::setPixel(int x, int y, const Color& color)
{
	m_image[y][x].color += color;
	m_image[y][x].nSamples++;
}

void glar::Image::setPixel(int index, const Color& color)
{
	if (index >= nPixels)
		throw std::invalid_argument("Argument value is invalid, must be in range (0, " + std::to_string(nPixels - 1) + ")");
	int x = index % width;
	int y = index / width;
	m_image[y][x].color += color;
	m_image[y][x].nSamples++;
}

glar::Pixel glar::Image::operator[](int index) const
{
	if(index>=nPixels)
		throw std::invalid_argument("Argument value is invalid, must be in range (0, " + std::to_string(nPixels-1) + ")");
	int x = index % width;
	int y = index / width;
	return m_image[y][x];
}

glar::Pixel& glar::Image::operator[](int index)
{
	if (index >= nPixels)
		throw std::invalid_argument("Argument value is invalid, must be in range (0, " + std::to_string(nPixels - 1) + ")");
	int x = index % width;
	int y = index / width;
	return m_image[y][x];
}


int glar::Image::getWidth() const
{
	return width;
}

int glar::Image::getHeight() const
{
	return height;
}

int glar::Image::numberOfPixels() const
{
	return nPixels;
}
