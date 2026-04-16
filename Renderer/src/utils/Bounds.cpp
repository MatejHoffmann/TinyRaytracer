#include "Bounds.h"
#include "Ray.h"

glar::Bounds::Bounds()
{
	double minNum = std::numeric_limits<double>::lowest();
	double maxNum = std::numeric_limits<double>::max();
	min = Point(maxNum, maxNum, maxNum);
	max = Point(minNum, minNum, minNum);
	centroid = getMidPoint(min, max);
}

glar::Bounds::Bounds(const Point& min, const Point& max)
	: min(min),
	max(max)
{
	centroid = getMidPoint(min, max);
}

bool glar::Bounds::intersect(const Ray& ray) const
{
	double tx1 = (min.x - ray.origin.x) * ray.i_direction.x;
	double tx2 = (max.x - ray.origin.x) * ray.i_direction.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (min.y - ray.origin.y) * ray.i_direction.y;
	double ty2 = (max.y - ray.origin.y) * ray.i_direction.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	double tz1 = (min.z - ray.origin.z) * ray.i_direction.z;
	double tz2 = (max.z - ray.origin.z) * ray.i_direction.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));
	
	return tmax >= tmin;
}

glar::Axis glar::Bounds::MaxExtent() const
{
	Vector diagonal = max - min;
	if (diagonal.x > diagonal.y && diagonal.x > diagonal.z)
		return Axis::x;
	else if (diagonal.y > diagonal.z)
		return Axis::y;
	else
		return Axis::z;
}

glar::Bounds glar::unionBounds(const Bounds& lhs, const Bounds& rhs)
{
	Point tmpMin = minPoint(lhs.min, rhs.min);
	Point tmpMax = maxPoint(lhs.max, rhs.max);
	return Bounds(tmpMin, tmpMax);
}

glar::Bounds glar::unionBounds(const Bounds& bounds, const Point& point)
{
	Point tmpMin = minPoint(bounds.min, point);
	Point tmpMax = maxPoint(bounds.max, point);
	return Bounds(tmpMin, tmpMax);
}

bool glar::compareCentroids(const Bounds& lhs, const Bounds& rhs, Axis axis)
{
	return lhs.centroid[axis] < rhs.centroid[axis];
}
