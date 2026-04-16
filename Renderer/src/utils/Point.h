/*****************************************************************//**
 * \file   Point.h
 * \brief  This file contains the implementation of Point class
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/
#pragma once
#include <ostream>
#include "Vector.h"

namespace glar
{
	/**
	 * this class is a representation of Point in 3D space. Almost all functions are pirmitive to understand and most of them are pirmitive.
	 */
	class Point
	{
	public:
		Point();

		Point(double s);

		Point(double x, double y, double z);

		Point(const Point& other);

		Point(Point&& other) noexcept;

		Point& operator=(Point other);

		friend void swap(Point& lhs, Point& rhs) noexcept;

		Point operator+(const Vector& vec) const;

		Point& operator+=(const Vector& vec);

		Point operator-(const Vector& vec) const;

		Vector operator-(const Point& p) const;

		Point& operator-=(const Vector& vec);

		Point operator*(double s) const;

		Point& operator*=(double s);

		Point operator/(double s) const;

		Point& operator/=(double s);

		double& operator[](int index);

		double operator[](int index) const;

		/**
		 * Distance between two given points.
		 */
		friend double distance(const Point& lhs, const Point& rhs);

		/**
		 * Squared distance between two points.
		 */
		friend double distanceSquared(const Point& lhs, const Point& rhs);

		friend bool operator==(const Point& lhs, const Point& rhs);

		friend bool operator!=(const Point& lhs, const Point& rhs);

		friend std::ostream& operator <<(std::ostream& os, const Point& point);

		/**
		 * Get the point int the middle of a line between two point.
		 */
		friend Point getMidPoint(const Point& lhs, const Point& rhs);

		/**
		 * creates a new point with lowest value of coordinates of the two points.
		 */
		friend Point minPoint(const Point& lhs, const Point& rhs);

		/**
		 * creates a new point with maximal value of coordinates of the two points.
		 */
		friend Point maxPoint(const Point& lhs, const Point& rhs);

		double x, y, z;

	};
}
