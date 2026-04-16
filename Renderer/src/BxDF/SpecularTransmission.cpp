#include "SpecularTransmission.h"

glar::SpecularTransmission::SpecularTransmission(const Color& color, const double ir)
:	BxDF(BxDFType(BxDFType::TRANSMISSION | BxDFType::SPECULAR)),
	color(color),
	ir(ir)
{
}

glar::Color glar::SpecularTransmission::f(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	return Color(0.0);
}

glar::Color glar::SpecularTransmission::sample(const Intersection& it, const Vector& wi, Vector& wo, double& pdf) const
{
	double ddot = dot(wi.normalized(), it.normal);
	double refractionRatio = ddot > 0.0 ? ir : 1.0 / ir;
	double sinTheta = std::sqrt(1.0 - ddot * ddot);

	pdf = 1.0;
	
	if (sinTheta * refractionRatio > 1.0)
		wo = reflect(wi, it.normal);
	else
	{
		if(ddot < 0)
			wo = refract(wi, -it.normal, refractionRatio);
		else
			wo = refract(wi, it.normal, refractionRatio);
	}
	return color;
}

double glar::SpecularTransmission::pdf(const Intersection& it, const Vector& wi, const Vector& wo) const
{
	return 0.0;
}
