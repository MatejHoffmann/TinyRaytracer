/*****************************************************************//**
 * \file   BxDF.h
 * \brief  This file contains implementation of an Abstract class BxDF, that serves as an interface for all implemented BRDF functions.
 * This class was inspired by the implementation of the renderer in the book Physically Based Rendering: from theory to implementation https://www.pbr-book.org/
 * 
 * \author Matěj
 * \date   May 2021
 *********************************************************************/

#pragma once
#include "../utils/Intersection.h"
#include "../utils/Sampler.h"


namespace glar
{	
	class Intersection;
	
	class BxDF
	{
	public:
		BxDF(BxDFType type) : type(type){}

		/**
		 * Counts the value of the brdf function from given incidnet and outgoing directions.
		 * 
		 * \param it	The point of reflection
		 * \param wi	The incident direction
		 * \param wo	The outgoing direction
		 * \return		The value of the brdf fucntion fro given parameters
		 */
		virtual Color f(const Intersection& it, const Vector& wi, const Vector& wo) const = 0;

		/**
		 * Samples random outgoing direction, in which the light can be reflected (or refracted), given the incident direction and counts the value of the BRDF fuctnio for given directions. 
		 * 
		 * \param it		The point of reflection(refraction)
		 * \param wi		The incident direction
		 * \param wo		The sampled outgoing direction
		 * \param pdf		the probability of the sampled direction
		 * \return			the brdf value for the given incident direction and sampled outgoing direction
		 */
		virtual Color sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const = 0;

		/**
		 * Probabily, that the surface described by the brdf will reflect the given incident direction in the direction of given vector wo
		 * 
		 * \param it		the point of reflection
		 * \param wi		the incident direction
		 * \param wo		the outgoing direction
		 * \return			the counted probability
		 */
		virtual double pdf(const Intersection& it, const Vector& wi, const Vector& wo) const = 0;
		virtual BxDFType getMaterialType() const { return type; }

		/**
		 * Find, whether the brdf matches flags given in the argument.
		 * 
		 * \param t	given flags
		 * \return	whether the brdf matches the given flags
		 */
		bool MatchesFlags(BxDFType t) const { return (type & t) == type; }
		BxDFType type;
	};
}

