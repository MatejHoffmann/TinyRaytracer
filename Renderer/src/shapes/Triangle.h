#pragma once
#include "../utils/Point.h"
#include "../utils/Vector.h"
#include "Primitive.h"

namespace glar
{
	class Vertex
	{
	public:
		Vertex() = default;

		Vertex(const Point& p, const Vector& n);

		Vertex(const Point& point);

		Point point;
		Vector normal;
	};


	class Triangle : public Primitive
	{
	public:

		Triangle(const Vertex& ve0, const Vertex& ve1, const Vertex& ve2, const std::shared_ptr<Material>& m_material, bool culling = true , bool recalculateNormals = false);

		Triangle(const Point& p0, const Point& p1, const Point& p2, const Vector& n0, const Vector& n1, const Vector& n2, const std::shared_ptr<Material>& m_material, bool culling = true);

		double area() const override;

		Intersection sample(double& pdf) const override;

		Vector getNormal(const Point& point) const override;

		bool intersect(const Ray& ray, Intersection& intersection) const override;

		bool boundsIntersect(const Ray& ray) const override;

		Bounds getBounds() const override;

		Vertex v0, v1, v2;

		Vector normal;

		Bounds bounds;

		bool culling;
		
	};
}
