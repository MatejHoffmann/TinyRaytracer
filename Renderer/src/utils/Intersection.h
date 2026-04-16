/*****************************************************************//**
 * \file   Intersection.h
 * \brief  Contains implementation of the Intersection class
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/

#pragma once
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include "../shapes/Primitive.h"

namespace glar
{
	class Primitive;

	/**
	 * This class contains information about the intersection.
	 */
	class Intersection
	{
	public:
		Intersection();

		Intersection(const Point& position, const Vector& normal);

		Intersection(const Point& point, const Vector& normal, const double t, const Vector& wo, const std::shared_ptr<Primitive>& primitive);

		//functions below compares the intersection based on the t parameter
		friend bool operator<(const Intersection& lhs, const Intersection& rhs);

		friend bool operator<=(const Intersection& lhs, const Intersection& rhs);

		friend bool operator>(const Intersection& lhs, const Intersection& rhs);

		friend bool operator>=(const Intersection& lhs, const Intersection& rhs);

		Point point;	//origin of the point

		Vector normal;	//normal of the surface at the poitn of intersection

		double t;		//parametr of the ray intersection ( distance from origin to the point)
		
		Vector wo;		// opposite direction of the ray
		
		std::shared_ptr<Primitive> primitive;
		
	};
}

