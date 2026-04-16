/*****************************************************************//**
 * \file   Scene.h
 * \brief	This file contains the implementaion of a Scene class
 * 
 * \author Matěj
 * \date   May 2021
 *********************************************************************/

#pragma once
#include "BVHTree.h"
#include "../utils/Ray.h"
#include "../shapes/Primitive.h"
#include "../lights/Light.h"

namespace glar
{
	class Light;
	
	/**
	 * This class represents the scene. It contain all the objects and other elements within the scene
	 */
	class Scene
	{
	public:
		// see BVHTree intersection
		bool intersection(const Ray& ray, Intersection& it) const;

		/**
		 * This function is a navie implementaion of a function for intersection checking. It loops through all the object in the scene and check the intersection with each of them
		 * 
		 * \param ray	the ray to be tested
		 * \param it	informtion abotu the found intersection point
		 * \return		wheter the intersection was found or not
		 */
		bool naiveIntersection(const Ray& ray, Intersection& it) const;

		/**
		 * Test whether the two point can see each other.
		 * 
		 * \param p0	first point
		 * \param p1	second point
		 * \return		whether the have a clear view on each other
		 */
		bool isVisible(const Point & p0, const Point & p1) const;

		/**
		 * Load obj scene from a given file path.
		 * 
		 * \param path
		 */
		void loadScene(const std::string& path);
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<Primitive>> primitives;
		std::vector<std::shared_ptr<Material>> materials;
		BVHTree bvhTree;
	};
}