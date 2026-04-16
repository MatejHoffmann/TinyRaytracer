/*****************************************************************//**
 * \file   Camera.h
 * \brief	This file contains the implementation of the Camera class
 * 
 * \author Matěj
 * \date   May 2021
 *********************************************************************/
#pragma once
#include "../utils/Ray.h"

namespace glar
{
	class Camera
	{
	public:

		/**
		 * Creates the camera and the local camera space fro given parameters.
		 * 
		 * \param width			width of the image in pixels
		 * \param height		height of the image in pixels
		 * \param p				the position of the camera
		 * \param look_at		the point this camera is centered on
		 * \param fov			field of view
		 */
		Camera(const int width, const int height, const Point& p, const Point& look_at, const double fov);
		/**
		 * Creates a ray, that is goind through the pixel with given indexes.
		 * 
		 * \param x		x coordinate of the pixel
		 * \param y		y coordintat of the pixel
		 * \return		the created ray
		 */
		Ray generateRay(int x, int y);

		/**
		 * This function is the same as the one above, except this work with the image as a continuous 1D array of pixels, intead of 2D.
		 * 
		 * \param index
		 * \return 
		 */
		Ray  generateRay(int index);
		int getHeight() const;
		int getWidth() const;
	private:
		int width, height, nPixels;
		Point position, lookAt, upperCorner;
		Vector u, n, v;
		double fov, aspect;
		double pixelSizeX, pixelSizeY;
	};
}

