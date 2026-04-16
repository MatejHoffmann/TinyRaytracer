#pragma once
#include "Light.h"

namespace glar
{
	class AreaLight : public Light
	{
	public:
		AreaLight(const Color& color, const double intensity, const int nSamples, const std::shared_ptr<Primitive>& m_pirimitive);

		bool sample(const Point& point, Vector& wi, Color& Le, double& pdf, const Scene& scene) const override;
		
		double pdf(const Point& point, const Vector& wi) const override;
		
		bool Le(const Point& point, const Vector& wi, Color& Le, const Scene& scene) const override;
	private:
		std::shared_ptr<Primitive> m_pirimitive;
	};
}