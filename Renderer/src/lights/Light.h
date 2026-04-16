/*****************************************************************//**
 * \file   Light.h
 * \brief  This file contains the implementation of abstract class Light, that serves as an Interface for all lights used in the system.
 * 
 * \author Mat?j
 * \date   May 2021
 *********************************************************************/
#pragma once
#include "../utils/Color.h"
#include "../utils/Point.h"
#include "../utils/Ray.h"
#include "../utils/Intersection.h"
#include "../core/Scene.h"


namespace glar
{
	class Scene;
	
	/**
	 * Abstract class Light. Serves as an Interface for all lights used in the system, defining its core functionality.
	 */
	class Light
	{
	public:

		Light(const Color& color, double intensity, int nSamples = 1) : m_color(color), m_intensity(intensity), m_nSamples(nSamples) {}


		/**
		 * Sample a random direction, from which a given point will be illuminated, if such a direction exists. Also, counts the color of the light arriving to the given point in the sampled direction and count the probability of this particular ray hitting the point (with respect to the point).
		 * 
		 * \param point		The point that is to be illuminated
		 * \param wi		The sampled direction
		 * \param Le		The color of the light
		 * \param pdf		The probability of sampling the ray
		 * \param scene		The scene in which all the objects are stored
		 * \return		
		 */
		virtual bool sample(const Point& point, Vector& wi, Color & Le, double& pdf, const Scene & scene) const = 0;

		// pdf of a light hitting given point in incident direction wi with respect to the point 

		/**
		 * Count the probability of a light ray hitting  the given point in the given direction. (with respect to solid angle)
		 * 
		 * \param point		The point that is to be illuminated
		 * \param wi		The direction we shoudl check 
		 * \return			the counted probability
		 */
		virtual double pdf(const Point& point, const Vector& wi) const = 0;

		// light emitted to the point in incident direction wi with respect to the point 

		/**
		 * color of a light emitted to the point in incident direction wi with respect to the point. The direction can be invalid(light will never hit the point in this direction) in that case returns false
		 * 
		 * \param point		The point that is to be illuminated
		 * \param wi		The incident direction
		 * \param Le		The color(radiance) of the light ray
		 * \param scene		The scene in which all the objects are stored
		 * \return			Whether the point is illuminated
		 */
		virtual bool Le(const Point & point, const Vector & wi, Color & Le, const Scene& scene) const = 0;

		virtual Color getColor() const { return m_color; }

		virtual double getIntensity() const { return m_intensity; }
		
		virtual int numberOfSamples() const { return m_nSamples; }

	protected:
		double m_intensity;
		Color m_color;
		int m_nSamples;
	};

}