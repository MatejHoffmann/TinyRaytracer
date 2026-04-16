/*****************************************************************//**
 * \file   Material.h
 * \brief  This file contains implementation of the Material class. 
 * The implementaions is inspired by the implementation of similar clas in PBR book  -- https://www.pbr-book.org/
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/

#pragma once
#include "../utils/Ray.h"
#include "../utils/Color.h"
#include "../utils/Intersection.h"
#include "../BxDF/BxDF.h"
#include <vector>

namespace glar
{
    class Intersection;
	class BxDF;
	enum BxDFType;

	/**
	 * This class serves as a wrapper around bunch of BRDFS, combining them and ultimatly creating the final looks of the material.
	 */
	class Material
	{
	public:
		/**
		 * Base constructor, all materials have emission atribute, that is by default set to 0.
		 * 
		 * \param emission
		 */
		Material(const Color& emission = Color(0.0));
		void addBxDF(std::shared_ptr<BxDF> bxdf);

		/**
		 * This function combines the all the f functions of all the contained brdfs. See explanation there.
		 */
        Color f(const Intersection& it, const Vector& wi, const Vector& wo, BxDFType type = BxDFType::ALL) const;
		/**
		 * This function combines the all the sample functions of all the contained brdfs. See explanation there.
		 */
        bool sample(const Intersection& it, const Vector& wi, Vector& wo, Color & f, double & pdf, BxDFType type = BxDFType::ALL) const;

		/**
		 * This function combines the all the pdff functions of all the contained brdfs. See explanation there.
		 */
        double pdf(const Intersection& it, const Vector& wi, const Vector& wo, BxDFType type = BxDFType::ALL) const;
		
        Color emission() const { return m_emission; }
		bool isEmissive() const;
		BxDFType getType() const { return m_type; }
		int NumOfComponents(BxDFType flags) const;
		int NumOfBxdfs() const;
	
	private:
		std::vector<std::shared_ptr<BxDF>> m_BxDFs;
		BxDFType m_type;								//contains the the types of brdfs, this material contains
		Color m_emission;								//the emission of the material, defaulted to 0
	};
}
