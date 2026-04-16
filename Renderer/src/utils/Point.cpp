#include "Point.h"
#include <stdexcept>

glar::Point::Point()
	: x(0),
	y(0),
	z(0)
{
}

glar::Point::Point(double s)
	: x(s),
	y(s),
	z(s)
{
}

glar::Point::Point(double x, double y, double z)
	: x(x),
	y(y),
	z(z)

{
}

glar::Point::Point(const Point& other)
	: x{ other.x },
	y{ other.y },
	z{ other.z }
{
}

glar::Point::Point(Point&& other) noexcept
	: x{ other.x },
	y{ other.y },
	z{ other.z }
{
}

glar::Point& glar::Point::operator=(Point other)
{
	using std::swap;
	swap(*this, other);
	return *this;
}

void glar::swap(Point& lhs, Point& rhs) noexcept
{
	using std::swap;
	swap(lhs.x, rhs.x);
	swap(lhs.y, rhs.y);
	swap(lhs.z, rhs.z);
}

glar::Point glar::Point::operator+(const Vector& vec) const
{
	return Point(x + vec.x, y + vec.y, z + vec.z);
}

glar::Point& glar::Point::operator+=(const Vector& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

glar::Point glar::Point::operator-(const Vector& vec) const
{
	return Point(x - vec.x, y - vec.y, z - vec.z);
}

glar::Vector glar::Point::operator-(const Point& p) const
{
	return Vector(x - p.x, y - p.y, z - p.z);
}

glar::Point& glar::Point::operator-=(const Vector& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

glar::Point glar::Point::operator*(double s) const
{
	return Point(x * s, y * s, z * s);
}

glar::Point& glar::Point::operator*=(double s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

glar::Point glar::Point::operator/(double s) const
{
	double inv = static_cast<double>(1) / s;
	return Point(x * inv, y * inv, z * inv);
}

glar::Point& glar::Point::operator/=(double s)
{
	double inv = static_cast<double>(1) / s;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

double& glar::Point::operator[](int index)
{
	if (index > 2 || index < 0)
		throw std::invalid_argument("Argument value is invalid, must be in range (0,2)");
	if (index == 0) return x;
	if (index == 1) return y;
	return z;
}

double glar::Point::operator[](int index) const
{
	if (index > 2 || index < 0)
		throw std::invalid_argument("Argument value is invalid, must be in range (0,2)");
	if (index == 0) return x;
	if (index == 1) return y;
	return z;
}

double glar::distance(const Point& lhs, const Point& rhs)
{
	return Vector(lhs - rhs).length();
}

double glar::distanceSquared(const Point& lhs, const Point& rhs)
{
	return Vector(lhs - rhs).lengthSquared();
}

bool glar::operator==(const Point& lhs, const Point& rhs)
{
	return lhs.x == rhs.x
		&& lhs.y == rhs.y
		&& lhs.z == rhs.z;
}

bool glar::operator!=(const Point& lhs, const Point& rhs)
{
	return !(lhs == rhs);
}

glar::Point glar::getMidPoint(const Point& lhs, const Point& rhs)
{
	return (Point(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z) / 2.0);
}

glar::Point glar::minPoint(const Point& lhs, const Point& rhs)
{
	return Point(std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y), std::min(lhs.z, rhs.z));
}

glar::Point glar::maxPoint(const Point& lhs, const Point& rhs)
{
	return Point(std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y), std::max(lhs.z, rhs.z));
}

namespace glar
{

	std::ostream& operator<<(std::ostream& os, const Point& p)
	{
		os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
		return os;
	}
}