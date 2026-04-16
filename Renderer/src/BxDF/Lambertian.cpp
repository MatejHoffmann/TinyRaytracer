#include "Lambertian.h"

glar::Lambertian::Lambertian(const Color& color)
:	BxDF(BxDFType(BxDFType::DIFFUSE | BxDFType::REFLECTION)),
	color(color)
{
}

glar::Color glar::Lambertian::f(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	return color * invPI;
}

glar::Color glar::Lambertian::sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const
{
	Sampler sampler;
	Vector v = sampler.cosineSampleHemisphere();
	Vector z = it.normal;
	Vector x, y;
	CoordinateSystem(z, x, y);

	wo = (x * v.x + y * v.y + z * v.z).normalize();
	pdf = Lambertian::pdf(it, wi, wo);
	return f(it, wi, wo);
}

double glar::Lambertian::pdf(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	double cosTheta = dot(wo, it.normal);
	return  cosTheta > 0.0 ? cosTheta * invPI : 0.0;
}
