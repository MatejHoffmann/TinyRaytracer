#include "Vector.h"
#include <stdexcept>

glar::Vector::Vector()
	: x(0),
	y(0),
	z(0)
{
}

glar::Vector::Vector(double s)
	: x(s),
	y(s),
	z(s)
{

}

glar::Vector::Vector(const double x, const double y, const double z)
	: x(x),
	y(y),
	z(z)
{
}

glar::Vector::Vector(const Vector& other)
	: x{ other.x },
	y{ other.y },
	z{ other.z }
{
}

glar::Vector::Vector(Vector&& other) noexcept
	: x{ other.x },
	y{ other.y },
	z{ other.z }
{
}

glar::Vector& glar::Vector::operator=(Vector other)
{
	using std::swap;
	swap(*this, other);
	return *this;
}

glar::Vector glar::Vector::operator+(const Vector& vec) const
{
	return Vector(x + vec.x, y + vec.y, z + vec.z);
}

glar::Vector& glar::Vector::operator+=(const Vector& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

glar::Vector glar::Vector::operator-(const Vector& vec) const
{
	return Vector(x - vec.x, y - vec.y, z - vec.z);
}

glar::Vector& glar::Vector::operator-=(const Vector& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

glar::Vector glar::Vector::operator*(double s) const
{
	return Vector(x * s, y * s, z * s);
}

glar::Vector& glar::Vector::operator*=(double s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

glar::Vector glar::Vector::operator/(double s) const
{
	double inv = static_cast<double>(1) / s;
	return Vector(x * inv, y * inv, z * inv);
}

glar::Vector& glar::Vector::operator/=(double s)
{
	double inv = static_cast<double>(1) / s;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

glar::Vector glar::Vector::operator-() const
{
	return Vector(-x, -y, -z);
}

double& glar::Vector::operator[](int index)
{
	if (index > 2 || index < 0)
		throw std::invalid_argument("Argument value is invalid, must be in range (0,2)");
	if (index == 0) return x;
	if (index == 1) return y;
	return z;
}

double glar::Vector::operator[](int index) const
{
	if (index > 2 || index < 0)
		throw std::invalid_argument("Argument value is invalid, must be in range (0,2)");
	if (index == 0) return x;
	if (index == 1) return y;
	return z;
}


double glar::Vector::lengthSquared() const
{
	return (x * x + y * y + z * z);
}

double glar::Vector::length() const
{
	return std::sqrt(lengthSquared());
}

glar::Vector& glar::Vector::normalize()
{
	if (length() != 0)
		*this /= length();
	return *this;
}

glar::Vector glar::Vector::normalized() const
{
	if (length() != 0)
		return  Vector(*this) / length();
	return Vector(*this);
}

double glar::dot(const Vector& lhs, const Vector& rhs)
{
	return (rhs.x * lhs.x + rhs.y * lhs.y + rhs.z * lhs.z);
}

glar::Vector glar::cross(const Vector& lhs, const Vector& rhs)
{
	double s_x = lhs.y * rhs.z - lhs.z * rhs.y;
	double s_y = lhs.z * rhs.x - lhs.x * rhs.z;
	double s_z = lhs.x * rhs.y - lhs.y * rhs.x;

	return Vector(s_x, s_y, s_z);
}

namespace glar
{

	std::ostream& operator<<(std::ostream& os, const Vector& vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
}
void glar::swap(Vector& lhs, Vector& rhs) noexcept
{
	using std::swap;
	swap(lhs.x, rhs.x);
	swap(lhs.y, rhs.y);
	swap(lhs.z, rhs.z);
}

bool glar::operator==(const Vector& lhs, const Vector& rhs)
{
	return lhs.x == rhs.x
		&& lhs.y == rhs.y
		&& lhs.z == rhs.z;
}

bool glar::operator!=(const Vector& lhs, const Vector& rhs)
{
	return !(lhs == rhs);
}
