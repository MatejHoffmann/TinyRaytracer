/*****************************************************************//**
 * \file   Sampler.h
 * \brief  This file contains the implementation of a Sampler class
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/
#pragma once
#include <random>

#include "Point.h"

namespace glar
{
	/**
	 * This class serves as a generator for random point, directions and numbers in the range of (0,1).
	 */
	class Sampler
	{
	public:
		Sampler();

		Sampler(const Sampler& other);

		Sampler(Sampler&& other) noexcept;

		Sampler& operator=(Sampler other);

		friend void swap(Sampler& lhs, Sampler& rhs) noexcept;
		
		Vector uniformSampleHemisphere();
		double uniformHemispherePdf();

		Vector exponentSampleHemisphere(double exp);
		Vector exponentHemispherePdf(double cosTheta, double exp);
		
		Vector uniformSampleSphere();
		double uniformSpherePdf();

		Vector uniformSampleCone(double cosThetaMax);
		double uniformConePdf(double cosThetaMax);

		Vector cosineSampleHemisphere();
		double cosineHemispherePdf(double cosTheta);

		double uniformSampleOne();
		double uniformSamplePdf(double a, double b);

		
		double powerHeuristic(int nf, double fPdf, double ng, double gPdf);

	private:
		std::mt19937 gen;							//random generator
		std::uniform_real_distribution<> dis;		//uniform distribution 
	};
}
