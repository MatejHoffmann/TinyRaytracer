#include "PointLight.h"

glar::PointLight::PointLight(const Color& color, const Point& position, const double intensity)
:   Light(color, intensity, 1),
    m_position(position)
{
}

bool glar::PointLight::sample(const Point& point, Vector& wi, Color& Le, double& pdf, const Scene& scene) const
{
	bool visibility = scene.isVisible(point, m_position);
	wi = (point - m_position).normalize();
	pdf = 1.0 * visibility;
	Le = ((m_color * m_intensity) / distanceSquared(point, m_position))*visibility;
	return  visibility;
}


double glar::PointLight::pdf(const Point& point, const Vector& wi) const
{
	return 0.0;
}

bool glar::PointLight::Le(const Point& point, const Vector& wi, Color& Le, const Scene& scene) const
{
	return false;
}
