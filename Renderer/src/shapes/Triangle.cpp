#include "Triangle.h"
#include "../core//Glar.h"
#include "../utils/Sampler.h"

glar::Vertex::Vertex(const Point& p, const Vector& n)
    : point(p),
    normal(n)
{
}

glar::Vertex::Vertex(const Point& point)
    : point(point),
    normal(Vector(0.0))
{
}

glar::Triangle::Triangle(const Vertex& ve0, const Vertex& ve1, const Vertex& ve2, const std::shared_ptr<Material>& material, bool culling, bool recalculateNormals)
    : Primitive(material),
    v0(ve0),
    v1(ve1),
    v2(ve2),
    culling(culling)
{
    double xmin = std::min(v0.point.x, std::min(v1.point.x, v2.point.x));
    double ymin = std::min(v0.point.y, std::min(v1.point.y, v2.point.y));
    double zmin = std::min(v0.point.z, std::min(v1.point.z, v2.point.z));

    double xmax = std::max(v0.point.x, std::max(v1.point.x, v2.point.x));
    double ymax = std::max(v0.point.y, std::max(v1.point.y, v2.point.y));
    double zmax = std::max(v0.point.z, std::max(v1.point.z, v2.point.z));


    bounds = Bounds(Point(xmin, ymin, zmin), Point(xmax, ymax, zmax));
    Vector edge1 = (v1.point - v0.point).normalize();
    Vector edge2 = (v2.point - v0.point).normalize();
    normal = cross(edge1,edge2).normalize();

    if (recalculateNormals)
    {
        v0.normal = normal;
        v1.normal = normal;
        v2.normal = normal;
    }

}

glar::Triangle::Triangle(const Point& p0, const Point& p1, const Point& p2, const Vector& n0, const Vector& n1, const Vector& n2, const std::shared_ptr<Material>& m_material, bool culling)
    : Primitive(m_material),
    v0(p0, n0),
    v1(p1, n1),
    v2(p2, n2),
	culling(culling)
{
    double xmin = std::min(p0.x, std::min(p1.x, p2.x));
    double ymin = std::min(p0.y, std::min(p1.y, p2.y));
    double zmin = std::min(p0.z, std::min(p1.z, p2.z));

    double xmax = std::max(p0.x, std::max(p1.x, p2.x));
    double ymax = std::max(p0.y, std::max(p1.y, p2.y));
    double zmax = std::max(p0.z, std::max(p1.z, p2.z));

    bounds = Bounds(Point(xmin, ymin, zmin), Point(xmax, ymax, zmax));
    normal = cross(v1.point - v0.point, v2.point - v0.point).normalize();
}

double glar::Triangle::area() const
{
    return 0.5 * cross(v1.point - v0.point, v2.point - v0.point).length();
}

glar::Intersection glar::Triangle::sample(double& pdf) const
{
    Sampler sampler;

    double s = std::sqrt(sampler.uniformSampleOne());
    double u = 1-s;
    double v = sampler.uniformSampleOne() * s;

    Intersection it;

    double x = v0.point.x * u + v1.point.x * v + v2.point.x * (1 - u - v);
    double y = v0.point.y * u + v1.point.y * v + v2.point.y * (1 - u - v);
    double z = v0.point.z * u + v1.point.z * v + v2.point.z * (1 - u - v);

    it.point = Point(x, y, z);
    Vector n = (v0.normal * u + v1.normal * v + v2.normal * (1 - u - v)).normalize();
    it.normal = dot(normal,n) < 0 ? -n : n;

    pdf = 1 / area();

    return it;
}

glar::Vector glar::Triangle::getNormal(const Point& point) const
{
    Vector edge1 = v1.point - v0.point;
    Vector edge2 = v2.point - v0.point;
    Vector toPoint = point - v0.point;

    double d11 = dot(edge1, edge1);
    double d12 = dot(edge1, edge2);
    double d22 = dot(edge2, edge2);
    double dp1 = dot(toPoint, edge1);
    double dp2 = dot(toPoint, edge2);

    double denom = d11 * d22 - d12 * d12;
    double invDenom = 1.0 / denom;
    double beta = (d22 * dp1 - d12 * dp2) * invDenom;
    double gamma = (d11 * dp2 - d12 * dp1) * invDenom;
    double alpha = 1.0 - beta - gamma;

    return Vector(v0.normal * alpha + v1.normal * beta + v2.normal * gamma).normalize();

}

bool glar::Triangle::intersect(const Ray& ray, Intersection& intersection) const
{
    Vector edge1 = v1.point - v0.point;
    Vector edge2 = v2.point - v0.point;
    Vector pVec = cross(ray.direction, edge2);
    double det = dot(edge1, pVec);

    if (culling)
    {
        if (det < EPSILON)
            return false;
    }
    else
    {
        if (std::abs(det) < EPSILON)
            return false;

    }

    double invDet = 1.0 / det;

    Vector tVec = ray.origin - v0.point;
    double beta = dot(tVec, pVec) * invDet;

    if (beta < 0.0 || beta> 1.0)
        return false;

    Vector qVec = cross(tVec, edge1);
    double gamma = dot(ray.direction, qVec) * invDet;

    if (gamma < 0.0 || gamma + beta > 1.0)
        return false;


    double t = dot(edge2, qVec) * invDet;
    double alpha = 1 - beta - gamma;


    if (t < EPSILON)
        return false;
	
    intersection.point = ray.getPoint(t);
	
    Vector n = Vector(v0.normal * alpha + v1.normal * beta + v2.normal * gamma).normalize();
    intersection.normal = dot(normal, n) < 0 ? -n : n;
    intersection.t = t;
    intersection.wo = -ray.direction;
    intersection.primitive = std::make_shared<Triangle>(*this);
	
    return true;
}


bool glar::Triangle::boundsIntersect(const Ray& ray) const
{
    return bounds.intersect(ray);
}

glar::Bounds glar::Triangle::getBounds() const
{
    return bounds;
}
