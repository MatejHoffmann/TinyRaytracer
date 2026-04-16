/*****************************************************************//**
 * \file   Ray.h
 * \brief  This file contains implementation of the Ray class
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/
#pragma once
#include "Point.h"
#include "Vector.h"
#include "../core/Glar.h"

namespace glar
{
	/**
	 * This class serves as a representation of a ray in 3D space.
	 */
	class Ray
	{
	public:
		Ray();

		/**
		 * Creates new ray from given point and direction.
		 * 
		 * \param origin	origin of the ray
		 * \param direction	directio nof the ray
		 */
		Ray(const Point& origin, const Vector& direction);

		/**
		 * Creates new ray from two points. 
		 * 
		 * \param origin		origin of the ray
		 * \param destination	destination point of the ray
		 */
		Ray(const Point& origin, const Point& destination);

		Ray(const Ray& other);

		Ray(Ray&& other) noexcept;

		Ray& operator=(Ray other);

		friend void swap(Ray& lhs, Ray& rhs) noexcept;

		friend std::ostream& operator <<(std::ostream& os, const Ray& ray);

		Point getPoint(double t) const;

		Point origin;			//origin of the ray

		Vector direction;		//direction of the ray

		Vector i_direction;		//inverse vector of the ray = 1/direction
	};
}

