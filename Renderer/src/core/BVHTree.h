/*****************************************************************//**
 * \file   BVHTree.h
 * \brief  This file contains implementaion of the BVH tree structure
 * 
 * \author Matěj
 * \date   May 2021
 *********************************************************************/

#pragma once
#include "../utils/Bounds.h"
#include <vector>
#include <memory>
#include "../shapes/Primitive.h"


namespace glar
{
	/**
	 * This class represents the node of the BVH tree.
	 */
	struct BVHNode
	{	
		~BVHNode();
		//creates leaf node
		void initLeaf(std::vector<std::shared_ptr<Primitive>> prims, const Bounds& b);

		//creates internal node
		void initInternal(Axis axis, BVHNode* leftChild, BVHNode* rightChild);
		BVHNode* left, * right;	//child nodes
		std::vector<std::shared_ptr<Primitive>> primitives;
		Bounds bounds; //bounds around all the abject containd in the primitive vector or all child nodes
		Axis splitAxis;	//the axis on which the boudning boxes were split
	};

	/**
	 * This class represents the BVH tree structure
	 */
	class BVHTree
	{
	public:
		BVHTree();
		~BVHTree();
		BVHTree(std::vector<std::shared_ptr<Primitive>> primitives);

		// public function that initiates the building of the tree
		void build(std::vector<std::shared_ptr<Primitive>> primitives);

		/**
		 * This function purpose is to find intersection of a ray with the object contained in the tree
		 * 
		 * \param ray			The ray to test the intersection with
		 * \param intersection	information about the intersection will be stored int this variable
		 * \return				whether the intersection was found
		 */
		bool intersect(const Ray& ray, Intersection& intersection) const;
	private:

		//	recursive allgorithm used in the intersect function
		bool hit(const Ray& ray, Intersection& intersection, BVHNode* node) const;

		// recursvie build function called from ht epublic pubild function
		BVHNode* recursiveBuild(std::vector<std::shared_ptr<Primitive>>& primitives, int start, int end);

		BVHNode* m_root;			//root of the tee
		std::vector<std::shared_ptr<Primitive>> m_primitives;
	};
}
