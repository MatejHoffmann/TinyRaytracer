/*****************************************************************//**
 * \file   Primitive.h
 * \brief  This files contains implementation of a abstract class Primitive
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/

#pragma once
#include <memory>
#include "../utils/Bounds.h"
#include "../materials/Material.h"
#include "../utils/Intersection.h"


namespace glar
{
	class Intersection;
	class Material;
	
	/**
	 * This class represents all geometrci objects that can be rendererd, and provies an interaface for its implementation.
	 */
	class Primitive
	{
	public:
		Primitive(const std::shared_ptr<Material>& m_material);

		/**
		 * All derived classes must implement this function. Its main purpose is to find an intersection between the geometric object and a ray.
		 * 
		 * \param ray				ray to test the intersection with
		 * \param intersection		saves the information about the intersectio nto this argument
		 * \return					whether the intersection occured or not
		 */
		virtual bool intersect(const Ray& ray, Intersection& intersection) const = 0;


		/**
		 * All derived classes must implement this function. Its main purpose is to find an intersection between the bounds of the geometric object and a ray.
		 *
		 * \param ray				ray to test the intersection with
		 * \return					whether the intersection occured or not
		 */
		virtual bool boundsIntersect(const Ray& ray) const = 0;


		virtual Bounds getBounds() const = 0;

		virtual std::shared_ptr<Material> getMaterial() const;

		virtual double area() const = 0;

		
		/**
		 * All derived classes must implement this function. Uniformly sample point on the surface of the object.
		 * 
		 * \param pdf		the probability of randomly and uniformly generating particular point
		 * \return			information about the sampled point -- its normal and position
		 */
		virtual Intersection sample(double& pdf) const = 0;

		/**
		 * Counts the probability of randomly and uniformly generating point
		 * 
		 * \return the counted probability
		 */
		virtual double pdf() const;

		// sample a point on the primitive, visible from the given point

		/**
		 * All derived classes must implement this function. Randomly generates a point on a surface, with respect of beeing observade from a praticular point. That means, that the sampled point should be visible from the point, and the probabilty of generating such a points given by a size of the solid angle, this object extend on the hemishere ardou nthe point.
		 * 
		 * \param point		the position, the primitive is observed from
		 * \param pdf		the probability of sampling such a direction
		 * \return			information about the sampled point -- its normal and position
		 */
		virtual Intersection sample(const Point& point, double& pdf);

		// pdf of observing this primitive from the given point in the given direction

		/**
		 * Counts the probability, that some point on the surface is observed from the given point
		 * 
		 * \param point		the position, the primitive is observed from
		 * \param wi		the direction in which the primitive is watched upon
		 * \return			the counted probability
		 */
		virtual double pdf(const Point& point, const Vector& wi);

		// get normal of a point on the surface
		virtual Vector getNormal(const Point& point) const = 0;

	protected:
		std::shared_ptr<Material> m_material;
	};

}
