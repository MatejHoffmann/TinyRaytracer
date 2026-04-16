#pragma once
#include "BxDF.h"
#include "../utils/Sampler.h"

namespace glar
{

	class Lambertian : public BxDF
	{
	public:
		Lambertian(const Color& color);
		Color f(const Intersection& it, const Vector& wi, const Vector& wo) const override;
		Color sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const override;
		double pdf(const Intersection& it, const Vector& wi, const Vector& wo) const override;
	private:
		Color color;
	};

}