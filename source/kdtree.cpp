#include "kdtree.h"

#include "mesh.h"

struct InnerNode
{
	size_t m_nodeData;
	float m_distance;
};

struct LeafNode
{
	size_t m_startIndex;
	size_t m_length;
};

union KdTree::Node {
	InnerNode m_inner;
	LeafNode m_leaf;
};

struct KdTree::Triangle
{
	size_t m_indices[3];
};

struct KdTree::Material
{
	unsigned m_placeholder;
};

void KdTree::Build(const Mesh& mesh)
{
	//mesh.
	//LeafNode root;
	//root.m_startIndex = 0;
	//root.m_length = mesh
}
