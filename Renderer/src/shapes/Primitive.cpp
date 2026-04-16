#include "Primitive.h"

glar::Primitive::Primitive(const std::shared_ptr<Material>& m_material)
	: m_material(m_material)
{
}

std::shared_ptr<glar::Material> glar::Primitive::getMaterial() const
{
	return m_material;
}

double glar::Primitive::pdf() const
{
	return 1 / area();
}

glar::Intersection glar::Primitive::sample(const Point& point, double& pdf)
{
	Intersection p = sample(pdf);
	Vector wi = (point - p.point).normalize();

	pdf *= distanceSquared(p.point, point) / std::abs(dot(p.normal, wi));

	if (std::isinf(pdf))
		pdf = 0;
	return p;
}

double glar::Primitive::pdf(const Point& point, const Vector& wi)
{
	Ray ray(point, -wi);
	Intersection it;
	if (!intersect(ray, it))
		return 0;

	double pdf = distanceSquared(point, it.point) / (std::abs((dot(it.normal, wi.normalized())) * area()));
	if (std::isinf(pdf))
		pdf = 0;
	return pdf;
}

