/*****************************************************************//**
 * \file   Bounds.h
 * \brief  Contains implementation of AABB 
 *
 * \author Mat?j Hoffmann
 * \date   May 2021
 *********************************************************************/
#pragma once
#include "Point.h"
#include "../core/Glar.h"
#include "../utils/Ray.h"

namespace glar
{
	
	class Bounds
	{
	public:
		/**
		 * Simple constructor, sets min to maximal possible positive number and max to minimal possible negative number
		 * 
		 */
		Bounds();
		/**
		 * 
		 * \param min sets min point
		 * \param max sets max point
		 */
		Bounds(const Point& min, const Point& max);

		/**
		 * .
		 * This functio checks, whether the given ray intersect the bounding box
		 * \param ray tests the intersection with this ray
		 * \return return true if intersection happens, otherwise false
		 */
		bool intersect(const Ray& ray) const;

		/**
		 * Mixes togther two bounds, creating possible larger bound aroudn more objects
		 */
		friend Bounds unionBounds(const Bounds& lhs, const Bounds& rhs);

		/**
		 * Extedns the bounds to contain additional point given in an argument
		 */
		friend Bounds unionBounds(const Bounds& bounds, const Point& point);

		/**
		 * compare the centorid coordinates of given axis of the tow given bounds
		 * \return true, when the lhs is smaller
		 */
		friend bool compareCentroids(const Bounds& lhs, const Bounds& rhs, Axis axis);

		/*
		* returns the axis, that has the largest span
		*/
		Axis MaxExtent() const;

		Point min, max, centroid; //points representing min and max of the bounds and centroid
	};

}