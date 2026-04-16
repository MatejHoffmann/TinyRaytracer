#include "BVHTree.h"
#include "BVHTree.h"

#include <algorithm>

glar::BVHNode::~BVHNode()
{
	if(left) delete left;
	if(right) delete right;
}

void glar::BVHNode::initLeaf(std::vector<std::shared_ptr<Primitive>> prims, const Bounds& b)
{
	std::swap(primitives, prims);
	bounds = b;
	left = right = nullptr;
}

void glar::BVHNode::initInternal(Axis axis, BVHNode* leftChild, BVHNode* rightChild)
{
	left = leftChild;
	right = rightChild;
	bounds = unionBounds(left->bounds, right->bounds);
	splitAxis = axis;
}

glar::BVHTree::BVHTree()
	: m_root(nullptr)
{
}

glar::BVHTree::~BVHTree()
{
	if(m_root) delete m_root;
}

glar::BVHTree::BVHTree(std::vector<std::shared_ptr<Primitive>> primitives)
	: m_primitives(std::move(primitives)),
	m_root(nullptr)
{
	if (m_primitives.empty())
		return;

	m_root = recursiveBuild(m_primitives, 0, m_primitives.size());
}

void glar::BVHTree::build(std::vector<std::shared_ptr<Primitive>> primitives)
{
	m_primitives = std::move(primitives);
	if (m_primitives.empty())
		return;

	m_root = recursiveBuild(m_primitives, 0, m_primitives.size());
}

glar::BVHNode* glar::BVHTree::recursiveBuild(std::vector<std::shared_ptr<Primitive>>& primitives, int start, int end)
{
	BVHNode* node = new BVHNode();

	Bounds bounds;
	for (int i = start; i < end; i++)
		bounds = unionBounds(bounds, m_primitives[i]->getBounds());

	int nPrimitives = end - start;
	if (nPrimitives == 1)
	{
		node->initLeaf({ primitives.begin() + start, m_primitives.begin() + end }, bounds);
		return node;
	}

	Bounds centroidBounds;

	for (int i = start; i < end; i++)
		centroidBounds = unionBounds(bounds, primitives[i]->getBounds().centroid);

	Axis dim = centroidBounds.MaxExtent();

	int mid = (start + end) / 2;

	if (centroidBounds.max[dim] == centroidBounds.min[dim])
	{
		node->initLeaf({ m_primitives.begin() + start, m_primitives.begin() + end }, bounds);
		return node;
	}

	double pMid = (centroidBounds.min[dim] + centroidBounds.max[dim]) / 2;

	std::vector<std::shared_ptr<Primitive>>::iterator midPtr = std::partition(primitives.begin() + start, primitives.begin() + end, [dim, pMid](const std::shared_ptr<Primitive>& pri) {return pri->getBounds().centroid[dim] < pMid; });

	mid = midPtr - primitives.begin();

	if (mid == start || mid == end)
	{
		mid = (start + end) / 2;
		std::nth_element(primitives.begin() + start, primitives.begin() + mid, primitives.begin() + end,
			[dim](const std::shared_ptr<Primitive>& a, const std::shared_ptr<Primitive>& b) {return a->getBounds().centroid[dim] < b->getBounds().centroid[dim]; });
	}

	node->initInternal(dim, recursiveBuild(primitives, start, mid), recursiveBuild(primitives, mid, end));

	return node;
}

bool glar::BVHTree::intersect(const Ray& ray, Intersection& intersection) const
{
	return hit(ray, intersection, m_root);
}

bool glar::BVHTree::hit(const Ray& ray, Intersection& intersection, BVHNode* node) const
{
	if (node == nullptr)
		return false;

	if (!node->bounds.intersect(ray))
		return false;

	if (node->left == nullptr && node->right == nullptr)
	{
		Intersection it;
		bool intersectionFound = false;
		for (auto primitive : node->primitives)
		{
			Intersection tmp;
			if (primitive->intersect(ray, tmp))
			{
				it = tmp < it ? tmp : it;
				intersectionFound = true;
			}
		}
		intersection = it;
		return intersectionFound;
	}

	Intersection leftIntersection;
	Intersection rightIntersection;

	bool l = hit(ray, leftIntersection, node->left);
	bool r = hit(ray, rightIntersection, node->right);

	if (l || r)
	{
		intersection = leftIntersection < rightIntersection ? leftIntersection : rightIntersection;
		return true;
	}
	return false;
}
