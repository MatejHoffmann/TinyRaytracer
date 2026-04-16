#include "Color.h"
#include <algorithm>
#include <stdexcept>
#include "Math.h"

glar::Color::Color()
	: r(1.0),
	g(1.0),
	b(1.0)
{
}

glar::Color::Color(double s)
	: r(s),
	g(s),
	b(s)
{
}

glar::Color::Color(const double r, const double g, const double b)
	: r(r),
	g(g),
	b(b)
{
}

void glar::Color::clamp()
{
	r = glar::clamp(0.0, 1.0, r);
	g = glar::clamp(0.0, 1.0, g);
	b = glar::clamp(0.0, 1.0, b);
}

glar::Color glar::mix(const Color& lhs, const Color& rhs, double weight)
{
	return lhs * (1 - weight) + rhs * weight;
}

glar::Color glar::mix(const Color& lhs, const Color& rhs, Color weight)
{
	return lhs * (Color(1.0) - weight) + rhs * weight;
}

bool glar::operator==(const Color& lhs, const Color& rhs)
{
	return lhs.r == rhs.r
		&& lhs.g == rhs.g
		&& lhs.b == rhs.b;
}

bool glar::operator!=(const Color& lhs, const Color& rhs)
{
	return !(lhs == rhs);
}

glar::Color glar::Color::operator*(double c) const
{
	return Color(r * c, g * c, b * c);
}

glar::Color& glar::Color::operator*=(double c)
{
	r *= c;
	g *= c;
	b *= c;
	return *this;
}

glar::Color glar::Color::operator*(Color c) const
{
	return Color(r * c.r, g * c.g, b * c.b);
}

glar::Color& glar::Color::operator*=(Color c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

glar::Color glar::Color::operator/(double s) const
{
	double inv = 1.0 / s;
	return Color(r * inv, g * inv, b * inv);
}

glar::Color& glar::Color::operator/=(double s)
{
	double inv = 1.0 / s;
	r *= inv;
	g *= inv;
	b *= inv;
	return *this;
}

glar::Color glar::Color::operator/(const Color& c) const
{
	return Color(r / c.r, g / c.g, b / c.b);
}

glar::Color& glar::Color::operator/=(const Color& c)
{
	r /= c.r;
	g /= c.g;
	b /= c.b;
	return *this;
}

glar::Color glar::Color::operator+(double s) const
{
	return Color(r + s, g + s, b + s);
}

glar::Color& glar::Color::operator+=(double s)
{
	r += s;
	g += s;
	b += s;
	return *this;
}

glar::Color glar::Color::operator+(const Color& c) const
{
	return Color(r + c.r, g + c.g, b + c.b);
}

glar::Color& glar::Color::operator+=(const Color& c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

glar::Color glar::Color::operator-(double s) const
{
	return Color(r - s, g - s, b - s);
}

glar::Color& glar::Color::operator-=(double s)
{
	r -= s;
	g -= s;
	b -= s;
	return *this;
}

glar::Color glar::Color::operator-(const Color& c) const
{
	return Color(r - c.r, g - c.g, b - c.b);
}

glar::Color& glar::Color::operator-=(const Color& c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

double& glar::Color::operator[](int index)
{
	if (index > 2 || index < 0)
		throw std::invalid_argument("Argument value is invalid, must be in range (0,2)");
	if (index == 0) return r;
	if (index == 1) return g;
	return b;
}

double glar::Color::operator[](int index) const
{
	if (index > 2 || index < 0)
		throw std::invalid_argument("Argument value is invalid, must be in range (0,2)");
	if (index == 0) return r;
	if (index == 1) return g;
	return b;
}

namespace glar
{

	std::ostream& operator<<(std::ostream& os, const Color& p)
	{
		os << "(" << p.r << ", " << p.g << ", " << p.b << ")";
		return os;
	}
}