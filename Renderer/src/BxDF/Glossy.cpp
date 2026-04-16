#include "Glossy.h"
#include "../utils/Sampler.h"

glar::Glossy::Glossy(const Color& color, double shininess)
:	BxDF(BxDFType(BxDFType::GLOSSY | BxDFType::REFLECTION)),
	color(color),
	shininess(shininess)
{
}

glar::Color glar::Glossy::f(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	Vector r = reflect(wi, it.normal);
	double RDotWo = std::max(0.0, dot(r, wo));
	return color * (shininess+2)*inv2PI*std::pow(RDotWo, shininess);
}

glar::Color glar::Glossy::sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const
{
	Vector r = reflect(wi, it.normal);
	Vector z = r.normalized();
	Vector x, y;
	CoordinateSystem(z, x, y);

	Sampler sampler;

	Vector v = sampler.exponentSampleHemisphere(shininess);
	
	wo = (x * v.x + y * v.y + z * v.z);

	if (dot(wo, it.normal) < 0.0)
		wo = (x * -v.x + y * -v.y + z * v.z);

	pdf = Glossy::pdf(it, wi, wo);
	return f(it, wi, wo);
}

double glar::Glossy::pdf(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	Vector r = reflect(wi, it.normal);
	double RDotWo = std::max(0.0, dot(r, wo));
	return std::pow(RDotWo, shininess) * ((shininess + 1) * inv2PI);
}
