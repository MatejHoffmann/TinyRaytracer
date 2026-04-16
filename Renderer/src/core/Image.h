/*****************************************************************//**
 * \file   Image.h
 * \brief	This files contains the implementation of a Image class.
 * 
 * \author Matěj
 * \date   May 2021
 *********************************************************************/
#pragma once
#include <vector>

#include "../utils/Color.h"
namespace glar
{
	/**
	 * This strcut acts as a pixel, saving the color, and how many times the pixel was sampled.
	 */
	struct Pixel
	{
		Pixel() :nSamples(0), color(Color(0.0)) {}
		int nSamples;
		Color color;
	};

	/**
	 * This class serves as a representation of an image.
	 */
	class Image
	{
	public:
		/**
		 * Creates new image with the given height and widht.
		 * 
		 * \param width		width of the image
		 * \param height	height of the image
		 */
		Image(const int width, const int height);

		/**
		 * Saves the image to a given file path.
		 * 
		 * \param path	the path to the file
		 */
		void saveImage(const std::string& path) const;

		Pixel getPixel(int x, int y) const;

		void setPixel(int x, int y, const Color& color);

		void setPixel(int index, const Color& color);

		Pixel operator[](int index) const;

		Pixel& operator[](int index);

		int getWidth() const;

		int getHeight() const;

		int numberOfPixels() const;

	private:
		int width, height, nPixels;
		std::vector<std::vector<Pixel>> m_image;
	};
}
