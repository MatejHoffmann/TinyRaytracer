/*****************************************************************//**
 * \file   Math.h
 * \brief  Contains few mathematical functions used in the program
 * 
 * \author Mat?j Hoffmann
 * \date   May 2021
 *********************************************************************/

#pragma once
#include "Vector.h"

namespace glar
{
	/**
	 * this function serves as a quadratic equation solver.
	 * 
	 * \param a parameter a of a quadratic equation
	 * \param b	parameter b of a quadratic equation
	 * \param c parameter c of a quadratic equation
	 * \param x0 contains the found root x1 of the a quadratic equation
	 * \param x1 contains the found root x2 of the a quadratic equation
	 * \return whether the equation is solvable in the real numbers domain
	 */
	inline bool solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1)
	{
		double discr = (b * b) - (4 * a * c);
		if (discr < 0)
		{
			return false;
		}
		else if (discr == 0)
		{
			x0 = x1 = -0.5 * (b / a);
		}
		else
		{
			double q = (b > 0) ? -0.5 * (b + std::sqrt(discr)) : -0.5 * (b - std::sqrt(discr));
			x0 = q / a;
			x1 = c / q;
		}
		return true;
	}
	/**
	 * Creates local coordinate system with the given vector v1.
	 * 
	 * \param v1 vector which is the new Z axis
	 * \param v2 vector which will be the new x asix
	 * \param v3 vector, which will be the new y axis
	 */
	inline void CoordinateSystem(const Vector& v1, Vector& v2, Vector& v3)
	{
		if (std::abs(v1.x) > std::abs(v1.y))
			v2 = Vector(-v1.z, 0, v1.x).normalize();
		else
			v2 = Vector(0, v1.z, -v1.y).normalize();
		v3 = cross(v1, v2);
	}

	/**
	 * Counts the reflection vector.
	 * 
	 * \param wi		incident vector	
	 * \param normal	normal at surface
	 * \return			the reflected vector
	 */
	inline Vector reflect(const Vector& wi, const Vector& normal)
	{
		return wi - normal * dot(wi, normal) * 2.0;
	}

	/**
	 * Counts the refraction vector.
	 * 
	 * \param wi		incident vector
	 * \param normal	normal of the surface
	 * \param ir		index of refraction
	 * \return			the refracted vector
	 */
	inline Vector refract(const Vector& wi, const Vector& normal, double ir)
	{
		Vector n = normal;
		Vector r = wi.normalized();
		
		double ddot = dot(r, n);

		double ior = 1.0 / ir;

		if (ddot < 0)
			ddot *= -1;
		else {
			n *= -1;
			ior = 1.0 / ior;
		}

		double minN = std::min(std::abs(ddot), 1.0);
		double k = 1 - ior * ior * (1.0 - minN * minN);
		if (k < 0.0)
			return Vector(0);

		return Vector(r * ior + n * (ior * minN - std::sqrt(k))).normalized();
	}


	/**
	 * Clamps given number by the given range.
	 * 
	 * \param min	minnimum of the range
	 * \param max	maximum of the range
	 * \param x		the number to be clamped
	 * \return		the value of clamped number
	 */
	inline double clamp(double min, double max, double x)
	{
		return std::min(max, std::max(x, min));
	}
	
}

