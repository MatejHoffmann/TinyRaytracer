#pragma once
#include "Primitive.h"

namespace glar
{

	class Sphere :public Primitive
	{
	public:

		Sphere(const Point& center, const double r, const std::shared_ptr<Material>& m_material);

		double area() const override;

		Intersection sample(double& pdf) const override;

		Intersection sample(const Point& point, double& pdf) override;

		double pdf(const Point& point, const Vector& wi) override;

		Vector getNormal(const Point& point) const override;

		bool intersect(const Ray& ray, Intersection& intersection) const override;

		bool boundsIntersect(const Ray& ray) const override;

		Bounds getBounds() const override;

	private:
		Point center;

		double r;

		Bounds bounds;

	};
}
