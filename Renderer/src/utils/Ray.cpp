#include "Ray.h"

glar::Ray::Ray()
{
}


glar::Ray::Ray(const Point& origin, const Vector& direction)
	: origin{ origin },
	direction{ direction.normalized() }
{
	this->origin += direction * EPSILON;
	double x = 1.0 / this->direction.x;
	double y = 1.0 / this->direction.y;
	double z = 1.0 / this->direction.z;
	i_direction = Vector(x, y, z);
}

glar::Ray::Ray(const Point& origin, const Point& destination)
:	origin(origin),
	direction((destination - origin).normalize())

{
	this->origin += direction * EPSILON;
	double x = 1.0 / this->direction.x;
	double y = 1.0 / this->direction.y;
	double z = 1.0 / this->direction.z;
	i_direction = Vector(x, y, z);
}


glar::Ray::Ray(const Ray& other)
	: origin{ other.origin },
	direction{ other.direction }
{
}

glar::Ray::Ray(Ray&& other) noexcept
	: origin{ std::move(other.origin) },
	direction{ std::move(other.direction) }
{
}

glar::Ray& glar::Ray::operator=(Ray other)
{
	using std::swap;
	swap(*this, other);
	return *this;
}

glar::Point glar::Ray::getPoint(double t) const
{
	return (origin + (direction * t));
}

void glar::swap(Ray& lhs, Ray& rhs) noexcept
{
	using std::swap;
	swap(lhs.origin, rhs.origin);
	swap(lhs.direction, rhs.direction);
}

namespace glar
{
	std::ostream& operator<<(std::ostream& os, const Ray& ray)
	{
		os << "origin: " << ray.origin << " direction: " << ray.direction;
		return os;
	}
}
