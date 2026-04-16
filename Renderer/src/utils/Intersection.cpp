#include "Intersection.h"

glar::Intersection::Intersection()
{
	t = std::numeric_limits<double>::max();
}

glar::Intersection::Intersection(const Point& position, const Vector& normal)
:	point(position),
	normal(normal)
{
	t = std::numeric_limits<double>::max();
}

glar::Intersection::Intersection(const Point& point, const Vector& normal, const double t, const Vector& wo, const std::shared_ptr<Primitive>& primitive)
:	point(point),
    normal(normal),
    t(t),
    wo(wo),
    primitive(primitive)
{
}

bool glar::operator<(const Intersection& lhs, const Intersection& rhs)
{
	return lhs.t < rhs.t;
}

bool glar::operator<=(const Intersection& lhs, const Intersection& rhs)
{
	return !(rhs < lhs);
}

bool glar::operator>(const Intersection& lhs, const Intersection& rhs)
{
	return rhs < lhs;
}

bool glar::operator>=(const Intersection& lhs, const Intersection& rhs)
{
	return !(lhs < rhs);
}

