#include "SpecularReflection.h"

glar::SpecularReflection::SpecularReflection(const Color& color)
:	BxDF(BxDFType(BxDFType::REFLECTION | BxDFType::SPECULAR)),
	color(color)
{
}

glar::Color glar::SpecularReflection::f(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	return Color(0);
}

glar::Color glar::SpecularReflection::sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const
{
	wo = reflect(wi, it.normal);
	pdf = 1.0;
	return color;
}

double glar::SpecularReflection::pdf(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	return 0.0;
}
