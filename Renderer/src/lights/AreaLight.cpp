#include "AreaLight.h"

glar::AreaLight::AreaLight(const Color& color, const double intensity, const int nSamples, const std::shared_ptr<Primitive>& m_pirimitive)
:   Light(color, intensity, nSamples),
    m_pirimitive(m_pirimitive)
{
}

bool glar::AreaLight::sample(const Point& point, Vector& wi, Color& Le, double& pdf, const Scene& scene) const
{
	Intersection it = m_pirimitive->sample(point, pdf);
	wi = (point - it.point).normalize();
	double ddot = dot(it.normal, wi);

	if (pdf == 0.0f || ddot <= 0)
	{
		pdf = 0.0;
		Le = Color(0.0f);
		return false;
	}
	
	bool visibility = scene.isVisible(point, it.point);

	pdf *= visibility;
	Le = ((m_color * m_intensity) / distanceSquared(it.point, point)) * ddot * visibility;
	return visibility;
}


double glar::AreaLight::pdf(const Point& point, const Vector& wi) const
{
	return m_pirimitive->pdf(point, wi);
}

bool glar::AreaLight::Le(const Point& point, const Vector& wi, Color& Le, const Scene& scene) const
{
	Intersection it;
	if (!m_pirimitive->intersect(Ray(point, -wi), it))
	{
		Le = Color(0.0f);
		return false;
	}

	bool visibility = scene.isVisible(point, it.point);
	double ddot = dot(wi, it.normal);
	bool emits = (visibility && ddot > 0.0f);
	Le = ((m_color * m_intensity * ddot) / distanceSquared(it.point, point)) * emits;
	return emits;
}

