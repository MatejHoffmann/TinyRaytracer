#include "Sphere.h"
#include "../core/Glar.h"
#include "../utils/Sampler.h"
#include "../utils/Math.h"

glar::Sphere::Sphere(const Point& center, const double r, const std::shared_ptr<Material>& material)
	: Primitive(material),
	center(center),
	r(r)
{
	Vector rOffset(r);
	bounds = Bounds(center - rOffset, center + rOffset);

}

double glar::Sphere::area() const
{
	return 4 * PI * r * r;
}

glar::Intersection glar::Sphere::sample(double& pdf) const
{
	Sampler sampler;
	
	Vector dir = sampler.uniformSampleSphere();
	Point p = center + dir.normalize() * r;
	
	pdf = 1 / area();

	return Intersection(p, getNormal(p));

}

glar::Intersection glar::Sphere::sample(const Point& point, double& pdf)
{
	//if point is inside the sphere
	if (distanceSquared(point, center) <= r * r)
	{
		Intersection p = sample(pdf);
		Vector wi = (point- p.point).normalize();
		pdf *= distanceSquared(p.point, point) / std::abs(dot(p.normal, wi));
		if (std::isinf(pdf))
			pdf = 0.0;
		return p;
	}

	double d = distance(point, center);
	double invD = 1 / d;
	Vector wc = (center - point) * invD;
	Vector wcX, wcY;
	glar::CoordinateSystem(wc, wcX, wcY);

	double sinThetaMax = r * invD;
	double sinThetaMax2 = sinThetaMax * sinThetaMax;
	double invSinThetaMax = 1 / sinThetaMax;
	double cosThetaMax = std::sqrt(std::max(0.0, 1 - sinThetaMax2));

	Sampler sampler;

	double u = sampler.uniformSampleOne();
	double v = sampler.uniformSampleOne();

	double cosTheta = (cosThetaMax - 1) * u + 1;
	double sinTheta2 = 1 - cosTheta * cosTheta;

	double cosAlpha = sinTheta2 * invSinThetaMax + cosTheta * std::sqrt(std::max(0.0, 1.0 - sinTheta2 * invSinThetaMax * invSinThetaMax));
	double sinAlpha = std::sqrt(std::max(0.0, 1.0 - cosAlpha * cosAlpha));
	double phi = v * 2 * PI;

	Vector shift = ((-wcX) * std::cos(phi) * sinAlpha + (-wcY) * std::sin(phi) * sinAlpha + (-wc) * cosAlpha).normalize();
	Point p = center + shift * r;

	pdf = 1.0 / (2.0 * PI * (1.0 - cosThetaMax));
	return Intersection(p, shift);

}

double glar::Sphere::pdf(const Point& point, const Vector& wi)
{
	if (distanceSquared(point, center) <= r * r)
		return Primitive::pdf(point, wi);

	double sinThetaMax2 = r * r / distanceSquared(point, center);
	double cosThetaMax = std::sqrt(std::max(0.0, 1.0 - sinThetaMax2));
	return (1.0 / (2.0 * PI * (1.0 - cosThetaMax)));
}


glar::Vector glar::Sphere::getNormal(const Point& point) const
{
	return Vector(point - center).normalize();
}

bool glar::Sphere::intersect(const Ray& ray, Intersection& intersection) const
{
	Vector L = ray.origin - center;
	double a = dot(ray.direction, ray.direction);
	double b = 2.0 * dot(ray.direction, L);
	double c = dot(L, L) - r * r;
	double t0, t1;
	if (!solveQuadratic(a, b, c, t0, t1))
		return false;

	if (t0 > t1)
		std::swap(t0, t1);

	if (t0 < EPSILON)
	{
		t0 = t1;
		if (t0 < EPSILON)
			return false;
	}

	intersection.point = ray.getPoint(t0);
	intersection.normal = getNormal(intersection.point);
	intersection.t = t0;
	intersection.wo = -ray.direction;
	intersection.primitive = std::make_shared<Sphere>(*this);
	
	return true;
}

bool glar::Sphere::boundsIntersect(const Ray& ray) const
{
	return bounds.intersect(ray);
}

glar::Bounds glar::Sphere::getBounds() const
{
	return bounds;
}
