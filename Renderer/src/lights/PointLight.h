#pragma once
#include "Light.h"

namespace glar
{
	class PointLight : public Light
	{
	public:
		PointLight(const Color& color, const Point& position, const double intensity);
		
		bool sample(const Point& point, Vector& wi, Color& Le, double& pdf, const Scene& scene) const override;
		
		double pdf(const Point& point, const Vector& wi) const override;
		
		bool Le(const Point& point, const Vector& wi, Color& Le, const Scene& scene) const override;
		
	private:
		Point m_position;
	};
}