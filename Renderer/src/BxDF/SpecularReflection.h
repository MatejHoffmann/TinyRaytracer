#pragma once
#include "BxDF.h"

namespace glar
{
	class SpecularReflection : public BxDF
	{
	public:
		SpecularReflection(const Color& color);
		Color f(const Intersection& it, const Vector& wi, const Vector& wo) const override;
		Color sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const override;
		double pdf(const Intersection& it, const Vector& wi, const Vector& wo) const override;
	private:
		Color color;
	};

}