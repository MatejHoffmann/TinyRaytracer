#include "Sampler.h"
#include "Sampler.h"
#include "../core/Glar.h"

glar::Sampler::Sampler()
{
	std::random_device random_device;
	gen = std::mt19937(random_device());
	dis = std::uniform_real_distribution<>(0.0f, 1.0f);
}

glar::Sampler::Sampler(const Sampler& other)
:	gen{ other.gen },
	dis{ other.dis }
{
}

glar::Sampler::Sampler(Sampler&& other) noexcept
	: gen{ std::move(other.gen) },
	dis{ std::move(other.dis) }
{
}

glar::Sampler& glar::Sampler::operator=(Sampler other)
{
	using std::swap;
	swap(*this, other);
	return *this;
}

glar::Vector glar::Sampler::uniformSampleHemisphere()
{
	double u = dis(gen);
	double v = dis(gen);

	double z = u;
	double r = std::sqrt(std::max(0.0, 1 - z * z));
	double phi = 2 * PI * v;
	return Vector(r * std::cos(phi), r * std::sin(phi), z);
}

double glar::Sampler::uniformHemispherePdf()
{
	return inv2PI;
}

glar::Vector glar::Sampler::exponentSampleHemisphere(double exp)
{
	double x = dis(gen);
	double y = dis(gen);

	double cos_phi = cos(2.0 * PI * x);
	double sin_phi = sin(2.0 * PI * x);
	double cos_theta = pow((1.0 - y), 1.0 / (exp + 1.0));
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	double pu = sin_theta * cos_phi;
	double pv = sin_theta * sin_phi;
	double pw = cos_theta;

	return Vector(pu, pv, pw);
}

glar::Vector glar::Sampler::exponentHemispherePdf(double cosTheta, double exp)
{
	return std::pow(cosTheta, exp)* (exp + 2)* inv2PI;
}

glar::Vector glar::Sampler::uniformSampleSphere()
{
	double u = dis(gen);
	double v = dis(gen);

	double z = 1-2*u;
	double r = std::sqrt(std::max(0.0, 1 - z * z));
	double phi = 2 * PI * v;
	return Vector(r * std::cos(phi), r * std::sin(phi), z);
}

double glar::Sampler::uniformSpherePdf()
{
	return inv4Pi;
}

glar::Vector glar::Sampler::uniformSampleCone(double cosThetaMax)
{
	double u = dis(gen);
	double v = dis(gen);
	
	double cosTheta = (1.0 - u) + u * cosThetaMax;
	double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
	double phi = v * 2 * PI;
	return Vector(std::cos(phi) * sinTheta, std::sin(phi) * sinTheta, cosTheta);
}

double glar::Sampler::uniformConePdf(double cosThetaMax)
{
	return 1.0 / (2.0 * PI * (1.0 - cosThetaMax));
}


glar::Vector glar::Sampler::cosineSampleHemisphere()
{
	double u = dis(gen);
	double v = dis(gen);


	double r = std::sqrt(u);
	double theta = 2 * PI * v;

	double x = r * std::cos(theta);
	double y = r * std::sin(theta);
	double z = std::sqrt(std::max(0.0, 1.0 - u));
	
	return Vector(x, y, z);
}

double glar::Sampler::cosineHemispherePdf(double cosTheta)
{
	return cosTheta * invPI;
}

double glar::Sampler::uniformSampleOne()
{
	return dis(gen);
}

double glar::Sampler::uniformSamplePdf(double a, double b)
{
	return 1.0 / (b - a);
}

double glar::Sampler::powerHeuristic(int nf, double fPdf, double ng, double gPdf)
{
	double f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

void glar::swap(Sampler& lhs, Sampler& rhs) noexcept
{
	using std::swap;
	swap(lhs.gen, rhs.gen);
	swap(lhs.dis, rhs.dis);
}
