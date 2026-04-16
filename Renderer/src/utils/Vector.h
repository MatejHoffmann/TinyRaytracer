/*****************************************************************//**
 * \file   Vector.h
 * \brief  This file contains implementation of the Vector class
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/

#pragma once
#include <ostream>

namespace glar {

	/**
	 * This class serves as a representation of a vector in 3D space and implements all operations between vectors.
	 */
	class Vector
	{
	public:
		Vector();

		Vector(double s);

		Vector(double x, double y, double z);

		Vector(const Vector& other);

		Vector(Vector&& other) noexcept;

		Vector& operator=(Vector other);

		friend void swap(Vector& lhs, Vector& rhs) noexcept;

		Vector operator+(const Vector& vec) const;

		Vector& operator+=(const Vector& vec);

		Vector operator-(const Vector& vec) const;

		Vector& operator-=(const Vector& vec);

		Vector operator*(double s) const;

		Vector& operator*=(double s);

		Vector operator/(double s) const;

		Vector& operator/=(double s);

		Vector operator-() const;

		double& operator[](int index);

		double operator[](int index) const;

		double length() const;

		double lengthSquared() const;

		Vector& normalize();

		Vector normalized() const;

		friend double dot(const Vector& lhs, const Vector& rhs);

		friend Vector cross(const Vector& lhs, const Vector& rhs);

		friend bool operator==(const Vector& lhs, const Vector& rhs);

		friend bool operator!=(const Vector& lhs, const Vector& rhs);

		friend std::ostream& operator <<(std::ostream& os, const Vector& vec);

		double x, y, z;
	};
}
