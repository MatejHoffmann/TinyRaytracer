/*****************************************************************//**
 * \file   Color.h
 * \brief  This file contains an implementation of Color class
 * 
 * \author Mat?j Hoffmann
 * \date   May 2021
 *********************************************************************/

#pragma once
#include <ostream>
#include "Math.h"

namespace glar
{
	/**
	 * This class serves as a representation for color most of the functions and operators are rather primitive
	 */
	class Color
	{
	public:
		Color();

		Color(double s);

		Color(const double r, const double g, const double b);
			
		/**
		 * clamps the color values to [0.0, 1.0]
		 * 
		 */
		void clamp();

		/**
		 * Mix the two colors, by the same weight on all components given in and argument
		 */
		friend Color mix(const Color& lhs, const Color& rhs, double weight);

		/**
		 * Mix the two colors, by a different weight on all components given in and argument
		 */
		friend Color mix(const Color& lhs, const Color& rhs, Color weight);

		Color operator*(double c) const;

		Color& operator *=(double c);

		Color operator*(Color c) const;

		Color& operator *=(Color c);

		Color operator/(double s) const;

		Color& operator /=(double s);

		Color operator/(const Color & c) const;

		Color& operator /=(const Color & c);

		Color operator+(double s) const;

		Color& operator+=(double s);

		Color operator+(const Color & c) const;

		Color& operator+=(const Color & c);

		Color operator-(double s) const;

		Color& operator-=(double s);

		Color operator-(const Color & c) const;

		Color& operator-=(const Color & c);

		friend bool operator==(const Color& lhs, const Color& rhs);

		friend bool operator!=(const Color& lhs, const Color& rhs);

		double& operator[](int index);

		double operator[](int index) const;

		friend std::ostream& operator <<(std::ostream& os, const Color & point);
		
		double r, g, b;
	};

}
